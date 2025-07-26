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
# echo "PARTUUID=d9c86127-03  /home/pi/music  ext4    defaults 0 2" >> /mnt/etc/fstab 
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

sudo apt install bluetooth pulseaudio* ~pulseaudio-module-bluetooth~



___


# binding buttons and stuff 
https://www.musicpd.org/doc/mpc/html/ 

https://github.com/eonpatapon/mpDris2
