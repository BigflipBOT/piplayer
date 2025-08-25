#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

void chck_n_print(int number, int pull) {
    int state = gpioRead(number);
    if (state == pull) {
        printf("GPIO%d is level %d \n", number, state);
    }
}

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
}

int main() {

    gpioCfgPermissions((1<<12) | (1<<23) |(1<<24) |(1<<5) |(1<<6) |(1<<7) |(1<<1) |(1<<20));
    gpioCfgClock(8, 1, 0);

    if (gpioInitialise() < 0) {
        return 1;
    } else {
        printf("pigpio initialized.\n");
    }

    gpio_setup();

    while (1) {
        // printf("GPIO24 is level %d", gpioRead(24));

        // screeen buttons
        chck_n_print(24, 0);
        chck_n_print(23, 0);

        // regular buttons
        chck_n_print(5, 1);
        chck_n_print(6, 1);
        chck_n_print(12, 1);

        // upper switch
        chck_n_print(7, 1);
        chck_n_print(1, 1);

        // bottom switch (swapped)
        chck_n_print(20, 1);
        chck_n_print(16, 1);

        // upper dipol
        chck_n_print(27, 1);
        chck_n_print(17, 1);

        // bottom dipol
        chck_n_print(13, 1);
        chck_n_print(26, 1);

        sleep(1);
    }

    // execl("/bin/ls", "ls", "-l", NULL);
    // while (1) {
    //     int pid = fork();
    //     int fd[2];
    //     printf("\33[2K\r");
    //     execl("/bin/mpc", NULL);
    // }
    return 0;
}
