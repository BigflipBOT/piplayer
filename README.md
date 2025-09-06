# Piplayer
This is my project that focuses on creating a music player out of Raspberry Pi zero 2w and basically other trash I found in my drower. This repo is a list of steps you have to do to get it working, basic schematics of control panel, some helper scripts and commands, and a simple program in C for gpio buttons support. If you like it you can try making your own :)

Currently work in progress, as i need to clean up this README and this repo in general.

### used parts list 
- (Raspberry Pi zero 2w)[https://example.com]
- (Adafriut Mini PITFT 1.3)[https://www.adafruit.com/product/4484]
- (Waveshare Li-ion Battery HAT)[https://www.waveshare.com/li-ion-battery-hat.htm]
- (PCM5102 DAC (or simmilar))[https://example.com]
- Universal doublesided Prototyping Board (3x7 cm)
- Tact Switches
- Slide switches (3 pos and 2 pos)
- DIP switch 2-pole
- Generic cables

# System Image tweaking 
generic tips and steps to prepare operating system as much as possible before.  
for this project raspberry pi os was choosen, however after a while i realised it was not the best choice.  
maybe one day custom operating system will be prepared with tool like yocto.
below actions are performed on your mashine. `#` indicates, that actions require root privelages.

## img download
https://www.raspberrypi.com/software/operating-systems/

## img customisation 
https://brettweir.com/blog/custom-raspberry-pi-image-no-hardware/

`/mnt/boot/firmware/cmdline.txt`
`/mnt/boot/firstrun.sh`

### chrooting into arm system from x86
usefull link:
https://wiki.archlinux.org/title/QEMU#Chrooting_into_arm/arm64_environment_from_x86_64

## flashing:
flashing image to the sd card
```
# xzcat 2025-05-13-raspios-bookworm-arm64-lite.img.xz | dd of=/dev/mmcblk0 bs=1M count=10240 status=progress
```

## creating new partition for music files
this step is technically optional, however it helps me with managing my music library.
### shrink fs as much as possible:
```
# e2fsck -f /dev/mmcblk0p2 && resize2fs -M /dev/mmcblk0p2
```

### (optional) check storage pace:
```
# dust /mnt
```

### changing partition table:
if you know what you are doing you can use any other tool. I will be using fdisk.
```
# fdisk /dev/mmcblk0
```

#### shrink p2
```
e 
2 
10G
```
#### list to get last sector:
```
p
Device         Boot  Start      End  Sectors  Size Id Type
/dev/mmcblk0p1        8192   532479   524288  256M  c W95 FAT32 (LBA)
/dev/mmcblk0p2      532480 21503999 20971520   10G 83 Linux
```
#### create new partition

```
n 
p 
<return>
2150400
<return>
```
#### (optional) list to verify everyting worked as intended:
```
p
Device         Boot    Start      End  Sectors  Size Id Type
/dev/mmcblk0p1          8192   532479   524288  256M  c W95 FAT32 (LBA)
/dev/mmcblk0p2        532480 21503999 20971520   10G 83 Linux
/dev/mmcblk0p3      21504000 62521343 41017344 19.6G 83 Linux
```

### resize filesystem on os partition to full partiton size:
```
# e2fsck -f /dev/mmcblk0p2 && resize2fs /dev/mmcblk0p2
```
# customisation of raspberry pi os
In this part you modify raspberry pi os. You need access to files, some actions you can perform while mounted, some actions require you having a shell access. Probably the easiest way is to set up the ssh connection to your pi however some .
## nonroot user customisation 
```
passwd pi 
usermod -aG sudo pi
```

## create music partition
```
mkdir /home/pi/music
# echo "PARTUUID=d9c86127-03  /home/pi/music  ext4    defaults 1 2" >> /mnt/etc/fstab 
# mkfs.ext4 /dev/mmcblk0p3
```
## upgrade and update
```
sudo apt-get -y update && sudo apt-get -y upgrade
```

https://www.instructables.com/Raspberry-Pi-HQ-Audio-PCM5102-and-MPD/
## blacklisting module
```
sudo vim /etc/modprobe.d/alsa-blacklist.conf
```
add: `blacklist snd_bcm2835`

## add driver support
```
sudo vim /boot/firmware/config.txt
```

comment out dtparam=audio=on (# in col 0)
add `dtoverlay=hifiberry-dac` at the end

## sound configuration

modify `/etc/asound.conf` file:
```
sudo vim /etc/asound.conf
```
add below lines:
```
pcm.!default {
type hw
card 0
}

ctl.!default {
type hw
card 0
}
```
add self to audio group:
```
sudo usermod -aG audio pi
```
reboot:
```
sudo reboot now
```

stuff should work by now, test with following commands:
```
aplay -l
speaker-test -D default -c 2 -twav
```


https://www.raspberrypi-spy.co.uk/2019/06/using-a-usb-audio-device-with-the-raspberry-pi/
sudo vim /usr/share/alsa/alsa.conf
```
defaults.ctl.card 1
defaults.pcm.card 1
```

https://www.hifiberry.com/docs/software/configuring-linux-3-18-x/

https://blog.himbeer.me/2018/12/27/how-to-connect-a-pcm5102-i2s-dac-to-your-raspberry-pi/




# bluetooth
https://forums.raspberrypi.com/viewtopic.php?t=306920
https://wiretuts.com/connecting-bluetooth-audio-device-to-raspberry-pi/
sudo usermod -aG bluetooth pi
sudo apt install pi-bluetooth bluez pulseaudio pulseaudio-module-bluetooth
sudo apt-get install pulseaudio*

https://unix.stackexchange.com/questions/705326/debian-11-bluetooth-sap-driver-initialization-failed
https://raspberrypi.stackexchange.com/questions/67617/bluetoothctl-fails-to-connect-to-any-device-failed-to-connect-org-bluez-erro
https://askubuntu.com/questions/689281/pulseaudio-can-not-load-bluetooth-module/689297#689297

```
sudo apt install bluetooth pulseaudio* ~pulseaudio-module-bluetooth~
```

## adafriut Mini PITFT 1.3
getting display working.  
usefull links:
(pinout of display)[https://learn.adafruit.com/adafruit-mini-pitft-135x240-color-tft-add-on-for-raspberry-pi?view=all]
(install instructions)[https://learn.adafruit.com/adafruit-mini-pitft-135x240-color-tft-add-on-for-raspberry-pi/1-3-240x240-kernel-module-install]
(driver repo)[https://github.com/adafruit/Raspberry-Pi-Installer-Scripts]

update and upgrade
```
sudo apt update -y && sudo apt-get update -y && sudo apt-get upgrade -y
```

Setup Virtual Environment
```
sudo apt install python3-venv
python -m venv env --system-site-packages
```

To activate the virtual environment:
```
source env/bin/activate
```

`sudo shutdown -h now` Remove power and attach the miniPiTFT. Watch that the pins plug into the first 2x12 headers! The rounded corner and mounting hole should line up.


Attach power to the Pi and re-log in. The PiTFT should be lit but nothing on the screen.
Run the following at the terminal

```
cd ~
pip3 install --upgrade adafruit-python-shell click
sudo apt-get install -y git
git clone https://github.com/adafruit/Raspberry-Pi-Installer-Scripts.git
cd Raspberry-Pi-Installer-Scripts
sudo -E env PATH=$PATH python3 adafruit-pitft.py --display=st7789_240x240 --rotation=3 --install-type=console
```

___

# installing and configuration of mpd and ncmpcpp
```
sudo apt-get install mpd ncmpcpp
systemctl --user ebable mpd
```

# controls
All gpio buttons emulate some keyboard buttons, depending on current layer selection
## input emulation
huge saver: (input-emulator)[https://github.com/tio/input-emulator], needs modification to support any different keyboard layout than dk, or changing keybord layout and keybinds.
linux input emulation (example)[https://github.com/torvalds/linux/blob/v4.12/include/uapi/linux/input-event-codes.h#L64] on github

## pigpio
you need to install (pigpio)[https://abyz.me.uk/rpi/pigpio/cif.html] library to use controls wrapper program for gpio buttons.
```
sudo apt install pigpio
```

## building programs
main build gcc command:
```
gcc -Wall -pthread -o prog prog.c -lpigpio -lrt
```
building helper script:
```
#!/bin/bash

filename=$(basename -- "$1")
filename=${filename%.*}
echo $filename
gcc -Wall -pthread -o $filename $1 -lpigpio -lrt
```
main program is being executed from .bashrc at home dir.

# optimalisations for battery life
## turning off unused stuff
https://ohyaan.github.io/tips/raspberry_pi_boot_time_optimization__complete_performance_guide/#2-kernel-command-line-optimization
https://www.cnx-software.com/2021/12/09/raspberry-pi-zero-2-w-power-consumption/

### config.txt modifications
```
# Restricting GPU
gpu_mem=16
# Turning off loading overlays for camera
camera_auto_detect=0
# Turning off loading overlays for dsi displys
display_auto_detect=0
# Disabling onboard audio
dtparam=audio=off
# (Comment out) force legacy drivers
#dtoverlay=vc4-kms-v3d
```

### usefull commands (didn't feel like doing those)
turning off hdmi 
https://forums.raspberrypi.com/viewtopic.php?t=113602
```
/usr/bin/tvservice -o
```
turning off leds
```
echo none | sudo tee /sys/class/leds/led0/trigger
```

