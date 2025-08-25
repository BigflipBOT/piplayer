## img dwnld
https://www.raspberrypi.com/software/operating-systems/

## img customisation 
https://brettweir.com/blog/custom-raspberry-pi-image-no-hardware/

`/mnt/boot/firmware/cmdline.txt`
`/mnt/boot/firstrun.sh`

### chrooting into arm system from x86
https://wiki.archlinux.org/title/QEMU#Chrooting_into_arm/arm64_environment_from_x86_64

## flashing:
```
xzcat 2025-05-13-raspios-bookworm-arm64-lite.img.xz | doas dd of=/dev/mmcblk0 bs=32M
```
```
xzcat 2025-05-13-raspios-bookworm-arm64-lite.img.xz | doas dd of=/dev/mmcblk0 bs=1M count=10240 status=progress
```

## shrink fs as much as possible:
```
doas e2fsck -f /dev/mmcblk0p2 && doas resize2fs -M /dev/mmcblk0p2
```

## sanity check for space:
```
doas dust /mnt
```

## changing partition table:
### shrink 
```
doas fdisk /dev/mmcblk0
e 
2 
10G
```
### list to get last sector:
```
p
Device         Boot  Start      End  Sectors  Size Id Type
/dev/mmcblk0p1        8192   532479   524288  256M  c W95 FAT32 (LBA)
/dev/mmcblk0p2      532480 21503999 20971520   10G 83 Linux
```
### create new partition

```
n 
p 
<return>
2150400
<return>
```
### list to verify:
```
p
Device         Boot    Start      End  Sectors  Size Id Type
/dev/mmcblk0p1          8192   532479   524288  256M  c W95 FAT32 (LBA)
/dev/mmcblk0p2        532480 21503999 20971520   10G 83 Linux
/dev/mmcblk0p3      21504000 62521343 41017344 19.6G 83 Linux
```


## resize filesystem to full partiton size:
```
doas e2fsck -f /dev/mmcblk0p2 && doas resize2fs /dev/mmcblk0p2
```
# customisation of raspi os

## nonroot user customisation 
```
passwd pi 
usermod -aG sudo pi

```

## hostname
~/etc/hostname~
already present: `raspberrypi`

## enable ssh:
maybe just systemctl start sshd???
```
doas touch /mnt/boot/ssh
```

## ~wpa_supplicant connection~
~maybe have to be turned on?~
legacy (not working):
```
# wpa_passphrase MYSSID passphrase > /mnt/etc/wpa_supplicant/wpa_supplicant.conf
```
## nmcli connection
`/etc/NetworkManager/system-connections/my-wifi.nmconnection`
```
doas chmod 600 wifi.nmconnection
doas chown root:root wifi.nmconnection
```

## create music partition
```
mkdir /home/pi/music
# echo "PARTUUID=d9c86127-03  /home/pi/music  ext4    defaults 1 2" >> /mnt/etc/fstab 
doas mkfs.ext4 /dev/mmcblk0p3
```
## giga umount
```
doas umount /dev/mmcblk0p3 /dev/mmcblk0p1 /mnt/proc /dev/mmcblk0p2
```
___

### ssh into pi

## upgrade and update
```
sudo apt-get -y update && sudo apt-get -y upgrade
```

https://www.instructables.com/Raspberry-Pi-HQ-Audio-PCM5102-and-MPD/
##
sudo nano /etc/modprobe.d/alsa-blacklist.conf

blacklist snd_bcm2835

## 
sudo vim /boot/firmware/config.txt

comment out dtparam=audio=on (# in col 0)
add `dtoverlay=hifiberry-dac` at the end

## sound configuration

sudo vim /etc/asound.conf
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
add self to audio group
```
sudo usermod -aG audio pi
```
reboot

stuff should work by now:
```
aplay -l
speaker-test -D default -c 2 -twav
```


https://www.raspberrypi-spy.co.uk/2019/06/using-a-usb-audio-device-with-the-raspberry-pi/
sudo nano /usr/share/alsa/alsa.conf
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

https://www.customelectronics.co.uk/rpihq.htm

# binding buttons and stuff 
## input emulation
huge saver:
https://github.com/tio/input-emulator
https://github.com/torvalds/linux/blob/v4.12/include/uapi/linux/input-event-codes.h#L64

https://github.com/chaitu236/TermKeyboard

https://forums.raspberrypi.com/viewtopic.php?t=87932
https://www.fsays.eu/Blogging/Blog/Details/23

https://forums.raspberrypi.com/viewtopic.php?t=284359

https://forums.raspberrypi.com/viewtopic.php?t=184050

## pigpio
https://abyz.me.uk/rpi/pigpio/cif.html
```
sudo apt install pigpio
```


https://www.musicpd.org/doc/mpc/html/ 

https://github.com/eonpatapon/mpDris2

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
main program is being executed from .bashrc at pi users home dir.

# optimalisation
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
### diasbling
