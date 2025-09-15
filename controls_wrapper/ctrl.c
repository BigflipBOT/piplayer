#include <errno.h>
#include <pigpio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void gpio_setup();
void interupt_handler(int);
static int running = 1;
void press_key();
void type_keys();
void gpio_debug(int gpio, int level, uint32_t tick);

int debug = 0;
void d_print(const char *str, ...);

typedef enum mode {
    ARROWS = 0,
    MUSIC_CONTROL = 1,
    G_NAVIGATION = 2,
} mode;
mode controls;
int ncmpcpp_screen = 1;
int override = 0;

void f_left_right(int gpio, int level, uint32_t tick);
void f_up_donw(int gpio, int level, uint32_t tick);
void f_play_pause_ok(int gpio, int level, uint32_t tick);
void f_slider_upper(int gpio, int level, uint32_t tick);
void f_slider_lower(int gpio, int level, uint32_t tick);
void f_dipol_upper(int gpio, int level, uint32_t tick);
int gpio27state = 0;
int gpio17state = 0;
void f_dipol_lower_l(int gpio, int level, uint32_t tick);
void f_dipol_lower_r(int gpio, int level, uint32_t tick);

// void testFunc(int gpio, int level, uint32_t tick) {
//     if (level == 1) {
//         printf("dupsko\n");
//     }
// }

int main(int argc, char *argv[]) {
    // int main() {
    // there is an assumption that following programs are already running:
    // mpd daemon
    // input-emulator daemon
    // ncmpcpp
    // struct sigaction act;
    // act.sa_handler = interupt_handler;
    // sigaction(SIGINT, &act, NULL);

    if (argc > 1) {
        debug = 1;
    }

    gpioCfgPermissions((1 << 12) | (1 << 23) | (1 << 24) | (1 << 5) | (1 << 6) |
                       (1 << 7) | (1 << 1) | (1 << 20));
    gpioCfgClock(8, 0, 0); // important to use PWM to evade audio issues

    if (gpioInitialise() < 0) {
        printf("pigpio failed.\n");
        exit(EXIT_FAILURE);
    }

    if (gpioRead(20)) {
        override = 1;
    }

    if (gpioRead(7)) {
        controls = G_NAVIGATION;
    } else if (gpioRead(1)) {
        controls = MUSIC_CONTROL;
    } else {
        controls = ARROWS;
    }

    if (gpioRead(27)) {
        gpio27state = 1;
        f_dipol_upper(27, gpio27state, 0);
    }
    if (gpioRead(17)) {
        gpio27state = 1;
        f_dipol_upper(17, gpio17state, 0);
    }

    gpioSetAlertFunc(12, f_play_pause_ok);

    gpioSetAlertFunc(23, f_left_right);
    gpioSetAlertFunc(24, f_left_right);

    gpioSetAlertFunc(5, f_up_donw);
    gpioSetAlertFunc(6, f_up_donw);

    gpioSetAlertFunc(7, f_slider_upper);
    gpioSetAlertFunc(1, f_slider_upper);

    gpioSetAlertFunc(20, f_slider_lower);

    // gpioSetAlertFunc(27, f_dipol_upper);
    // gpioSetAlertFunc(17, f_dipol_upper);

    d_print("init done\n");
    d_print("mode %d\n", controls);
    if (override) {
        d_print("override active \n");
    }

    // main loop
    while (running) {
        int tempstate;

        // handled that way beacuse of hardware switches limitations...
        tempstate = gpioRead(27);
        if (tempstate != gpio27state){
            gpio27state = tempstate;
            f_dipol_upper(27, gpio27state, 0);
        }
        tempstate = gpioRead(17);
        if (tempstate != gpio17state){
            gpio17state = tempstate;
            f_dipol_upper(17, gpio17state, 0);
        }

        sleep(2);
    }

    /*   d_print("terminating...\n");
       gpioTerminate();
       exit(EXIT_SUCCESS); */
}

void f_left_right(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        char *arrow;
        char *music_ctrl;
        if (gpio == 23) {
            arrow = "left";
            music_ctrl = "<";
        } else if (gpio == 24) {
            arrow = "right";
            music_ctrl = ">";
        }

        switch (controls) {
        case ARROWS:
            press_key(arrow);
            break;
        case MUSIC_CONTROL:
            press_key(music_ctrl);
            break;
        case G_NAVIGATION:
            if (gpio == 24) {
                if (ncmpcpp_screen < 7) {
                    ncmpcpp_screen++;
                }
            } else if (gpio == 23) {
                if (ncmpcpp_screen > 1) {
                    ncmpcpp_screen--;
                }
            }
            char str[2];
            str[0] = '0' + ncmpcpp_screen;
            str[1] = '\0';
            press_key(str);
            break;
        }
    }
    return;
}

void f_up_donw(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        char *arrow;
        char *music_ctrl;
        if (gpio == 5) {
            arrow = "up";
            music_ctrl = "=";
        } else if (gpio == 6) {
            arrow = "down";
            music_ctrl = "-";
        }
        switch (controls) {
        case ARROWS:
            press_key(arrow);
            if (override) {
                for (int i = 0; i < 14; i++) {
                    press_key(arrow);
                }
            }
            break;
        case MUSIC_CONTROL:
            press_key(music_ctrl);
            break;
        case G_NAVIGATION:
            press_key(arrow);
            break;
        }
    }
    return;
}
void f_play_pause_ok(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        switch (controls) {
        case ARROWS:
            if (override) {
                press_key("a");
            } else {
                press_key("enter");
            }
            break;
        case MUSIC_CONTROL:
            press_key("p");
            break;
        case G_NAVIGATION:
            if (override) {
                type_keys("`s20");
                press_key("enter");
            } else {
                press_key("delete");
            }
            break;
        }
    }
    return;
}

void f_slider_upper(int gpio, int level, uint32_t tick) {
    d_print("f_slider_upper invoked\n");
    if (level == 2) {
        d_print("slider_upper error: level 2\n");
        exit(EXIT_FAILURE);
    } else if (level == 1) {
        switch (gpio) {
        case 1:
            controls = MUSIC_CONTROL;
            d_print("mode: %d\n", MUSIC_CONTROL);
            break;
        case 7:
            controls = G_NAVIGATION;
            d_print("mode: %d\n", G_NAVIGATION);
        }
    } else if (level == 0) {
        controls = ARROWS;
        d_print("mode: %d\n", ARROWS);
    }
    return;
}

void f_slider_lower(int gpio, int level, uint32_t tick) {
    switch (gpio) {
    case 20:
        if (level == 0) {
            override = 0;
        } else if (level == 1) {
            override = 1;
        }
        break;

    case 13:
        break;
    }
    if (override) {
        d_print("override activated\n");
    } else {
        d_print("override deactivated\n");
    }
    return;
}

void f_dipol_upper(int gpio, int level, uint32_t tick) {
    // d_print("f_dipol_upper invoked\n");
    char *component;
    char *action;
    switch (gpio) {
    case 27:
        component = "wlan";
        break;
    case 17:
        component = "bluetooth";
        break;
    }
    switch (level) {
    case 1:
        action = "unblock";
        break;
    case 0:
        action = "block";
        break;
    }

    int pid;
    d_print("executing rfkill %s %s\n", action, component);

    if ((pid = fork()) == 0) {
        execl("/usr/sbin/rfkill", "rfkill", action, component, NULL);
        d_print("execl error %s\n", strerror(errno));
        exit(1);
    }
}

// void f_dipol_upper_r(int gpio, int level, uint32_t tick);

void f_dipol_lower_l(int gpio, int level, uint32_t tick);

void f_dipol_lower_r(int gpio, int level, uint32_t tick);

void press_key(char *input) {
    int pid;
    // printf("\33[2K\r");
    if ((pid = fork()) == 0) {
        d_print("pressing: %s\n", input);
        execl("/usr/local/bin/input-emulator", "input-emulator", "kbd", "key",
              input, NULL);
        d_print("execl error %s\n", strerror(errno));
        exit(1);
    }
}

void type_keys(char *input) {
    int pid;
    if ((pid = fork()) == 0) {
        d_print("pressing: %s\n", input);
        execl("/usr/local/bin/input-emulator", "input-emulator", "kbd", "type",
              input, NULL);
        d_print("execl error %s\n", strerror(errno));
        exit(1);
    }
}

void d_print(const char *str, ...) {
    va_list args;
    va_start(args, str);
    if (debug > 0) {
        vprintf(str, args);
    }
}

void gpio_debug(int gpio, int level, uint32_t tick) {
    printf("haha, johnatan, i am being executed! gpio: %d, level: %d\n", gpio,
           level);
}
// void interupt_handler(int) { running = 0; }

void gpio_setup() {
    gpioSetMode(5, PI_INPUT);
    gpioSetMode(6, PI_INPUT);
    gpioSetMode(7, PI_INPUT);
    gpioSetMode(1, PI_INPUT);
    gpioSetMode(12, PI_INPUT);
    gpioSetMode(20, PI_INPUT);
    gpioSetMode(16, PI_INPUT);
    gpioSetMode(27, PI_INPUT);
    gpioSetMode(17, PI_INPUT);
    gpioSetMode(13, PI_INPUT);
    gpioSetMode(26, PI_INPUT);
    gpioSetMode(24, PI_INPUT);
    gpioSetMode(23, PI_INPUT);
    gpioSetMode(22, PI_OUTPUT);

    gpioSetPullUpDown(5, PI_PUD_DOWN);
    gpioSetPullUpDown(6, PI_PUD_DOWN);
    gpioSetPullUpDown(7, PI_PUD_DOWN);
    gpioSetPullUpDown(1, PI_PUD_DOWN);
    gpioSetPullUpDown(12, PI_PUD_DOWN);
    gpioSetPullUpDown(20, PI_PUD_DOWN);
    gpioSetPullUpDown(16, PI_PUD_DOWN);
    gpioSetPullUpDown(27, PI_PUD_DOWN);
    gpioSetPullUpDown(17, PI_PUD_DOWN);
    gpioSetPullUpDown(13, PI_PUD_DOWN);
    gpioSetPullUpDown(26, PI_PUD_DOWN);
    gpioSetPullUpDown(24, PI_PUD_UP);
    gpioSetPullUpDown(23, PI_PUD_UP);
    gpioSetPullUpDown(22, PI_PUD_DOWN);
}
