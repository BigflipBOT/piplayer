#!/bin/bash
set +e

CURRENT_HOSTNAME=BigflipBOT
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
   /usr/lib/raspberrypi-sys-mods/imager_custom set_hostname raspberrypi
else
   echo raspberrypi >/etc/hostname
   sed -i "s/127.0.1.1.*/127.0.1.1	raspberrypi/g" /etc/hosts
fi
FIRSTUSER=filip
FIRSTUSERHOME=/home/filip
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
   /usr/lib/raspberrypi-sys-mods/imager_custom enable_ssh -k 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDuCA2R0gLlB2fj3eKHCaFdvqpKrFq0Iq/HJOPcCNY0sesm6FAFHwhwQNruwsGy0KJYx06/hNDENJNtvuxp6ANMzY6q381SHB4pkdJWOB6tIAR80bkH81Objf+ha132vFpxl8L67gJmHskGcGLYp39LAhk8YIaZSzFOlveGV9WPaGYvO8TxfV0SKqzbz8MTscCBNMyHGWCOD4cV9hYrj5ST6KbQM73xyEUQEUKtzLx0i8qioARTtTGG4bHM2SOKclbbyE6fec6cEjzfom3sXVv2KPIUMFLXCa9hXbPaW5Ixou30YF5NIz2yelPoFIUPlgvcD+GOWG+kPkDkXAUVISHrFkQSHindZS90SnlSyWAaV2n5F5YS18MqEajBwKLRcryVki/zMO6Dkl72d8nvTE/jjDAUaKveR9OXlCZ+vyQFdXD2OpCRJJwhkCRNU89a24696vJtlunl32NmMRgf5KvWp3T6e9vbPRieDXUEIqPN55zfKOlOoS2bgWemCFz9/0EuRaW43t6hQk5uzHyK4+Ozqa46xeCQxFj7QQrMPyciZ93nZAxpRv6+5hjVYlLeo8leUMoPI4+XDBD3OrzqoAX+anQ4dw0tH/6ReQaSvrL0C9CsEDxwNFrMARahs2EG9khq9afSbZczQj5chSl7AI+bRWY/nYStKtBwHvbf0ZFKDQ== flipypr@protonmail.com' 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDuCA2R0gLlB2fj3eKHCaFdvqpKrFq0Iq/HJOPcCNY0sesm6FAFHwhwQNruwsGy0KJYx06/hNDENJNtvuxp6ANMzY6q381SHB4pkdJWOB6tIAR80bkH81Objf+ha132vFpxl8L67gJmHskGcGLYp39LAhk8YIaZSzFOlveGV9WPaGYvO8TxfV0SKqzbz8MTscCBNMyHGWCOD4cV9hYrj5ST6KbQM73xyEUQEUKtzLx0i8qioARTtTGG4bHM2SOKclbbyE6fec6cEjzfom3sXVv2KPIUMFLXCa9hXbPaW5Ixou30YF5NIz2yelPoFIUPlgvcD+GOWG+kPkDkXAUVISHrFkQSHindZS90SnlSyWAaV2n5F5YS18MqEajBwKLRcryVki/zMO6Dkl72d8nvTE/jjDAUaKveR9OXlCZ+vyQFdXD2OpCRJJwhkCRNU89a24696vJtlunl32NmMRgf5KvWp3T6e9vbPRieDXUEIqPN55zfKOlOoS2bgWemCFz9/0EuRaW43t6hQk5uzHyK4+Ozqa46xeCQxFj7QQrMPyciZ93nZAxpRv6+5hjVYlLeo8leUMoPI4+XDBD3OrzqoAX+anQ4dw0tH/6ReQaSvrL0C9CsEDxwNFrMARahs2EG9khq9afSbZczQj5chSl7AI+bRWY/nYStKtBwHvbf0ZFKDQ== flipypr@protonmail.com'
else
   install -o "" -m 700 -d "/.ssh"
   install -o "" -m 600 <(printf "'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDuCA2R0gLlB2fj3eKHCaFdvqpKrFq0Iq/HJOPcCNY0sesm6FAFHwhwQNruwsGy0KJYx06/hNDENJNtvuxp6ANMzY6q381SHB4pkdJWOB6tIAR80bkH81Objf+ha132vFpxl8L67gJmHskGcGLYp39LAhk8YIaZSzFOlveGV9WPaGYvO8TxfV0SKqzbz8MTscCBNMyHGWCOD4cV9hYrj5ST6KbQM73xyEUQEUKtzLx0i8qioARTtTGG4bHM2SOKclbbyE6fec6cEjzfom3sXVv2KPIUMFLXCa9hXbPaW5Ixou30YF5NIz2yelPoFIUPlgvcD+GOWG+kPkDkXAUVISHrFkQSHindZS90SnlSyWAaV2n5F5YS18MqEajBwKLRcryVki/zMO6Dkl72d8nvTE/jjDAUaKveR9OXlCZ+vyQFdXD2OpCRJJwhkCRNU89a24696vJtlunl32NmMRgf5KvWp3T6e9vbPRieDXUEIqPN55zfKOlOoS2bgWemCFz9/0EuRaW43t6hQk5uzHyK4+Ozqa46xeCQxFj7QQrMPyciZ93nZAxpRv6+5hjVYlLeo8leUMoPI4+XDBD3OrzqoAX+anQ4dw0tH/6ReQaSvrL0C9CsEDxwNFrMARahs2EG9khq9afSbZczQj5chSl7AI+bRWY/nYStKtBwHvbf0ZFKDQ== flipypr@protonmail.com'
'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDuCA2R0gLlB2fj3eKHCaFdvqpKrFq0Iq/HJOPcCNY0sesm6FAFHwhwQNruwsGy0KJYx06/hNDENJNtvuxp6ANMzY6q381SHB4pkdJWOB6tIAR80bkH81Objf+ha132vFpxl8L67gJmHskGcGLYp39LAhk8YIaZSzFOlveGV9WPaGYvO8TxfV0SKqzbz8MTscCBNMyHGWCOD4cV9hYrj5ST6KbQM73xyEUQEUKtzLx0i8qioARTtTGG4bHM2SOKclbbyE6fec6cEjzfom3sXVv2KPIUMFLXCa9hXbPaW5Ixou30YF5NIz2yelPoFIUPlgvcD+GOWG+kPkDkXAUVISHrFkQSHindZS90SnlSyWAaV2n5F5YS18MqEajBwKLRcryVki/zMO6Dkl72d8nvTE/jjDAUaKveR9OXlCZ+vyQFdXD2OpCRJJwhkCRNU89a24696vJtlunl32NmMRgf5KvWp3T6e9vbPRieDXUEIqPN55zfKOlOoS2bgWemCFz9/0EuRaW43t6hQk5uzHyK4+Ozqa46xeCQxFj7QQrMPyciZ93nZAxpRv6+5hjVYlLeo8leUMoPI4+XDBD3OrzqoAX+anQ4dw0tH/6ReQaSvrL0C9CsEDxwNFrMARahs2EG9khq9afSbZczQj5chSl7AI+bRWY/nYStKtBwHvbf0ZFKDQ== flipypr@protonmail.com'
") "/.ssh/authorized_keys"
   echo 'PasswordAuthentication no' >>/etc/ssh/sshd_config
   systemctl enable ssh
fi
if [ -f /usr/lib/userconf-pi/userconf ]; then
   /usr/lib/userconf-pi/userconf 'pi' '/cEYc.wVIL1ewUyyzZLiBrVCtR6'
else
   echo ":"'/cEYc.wVIL1ewUyyzZLiBrVCtR6' | chpasswd -e
   if [ "" != "pi" ]; then
      usermod -l "pi" ""
      usermod -m -d "/home/pi" "pi"
      groupmod -n "pi" ""
      if grep -q "^autologin-user=" /etc/lightdm/lightdm.conf ; then
         sed /etc/lightdm/lightdm.conf -i -e "s/^autologin-user=.*/autologin-user=pi/"
      fi
      if [ -f /etc/systemd/system/getty@tty1.service.d/autologin.conf ]; then
         sed /etc/systemd/system/getty@tty1.service.d/autologin.conf -i -e "s//pi/"
      fi
      if [ -f /etc/sudoers.d/010_pi-nopasswd ]; then
         sed -i "s/^ /pi /" /etc/sudoers.d/010_pi-nopasswd
      fi
   fi
fi
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
   /usr/lib/raspberrypi-sys-mods/imager_custom set_wlan 'Akacjowa 7E_5G' '883fafa80cde797bd09c637dbc97ec6c99bf12d42d07d267b64edd70438380b7' 'PL'
else
cat >/etc/wpa_supplicant/wpa_supplicant.conf <<'WPAEOF'
country=PL
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
ap_scan=1

update_config=1
network={
	scan_ssid=1
	ssid="Akacjowa 7E_5G"
	psk=883fafa80cde797bd09c637dbc97ec6c99bf12d42d07d267b64edd70438380b7
}

WPAEOF
   chmod 600 /etc/wpa_supplicant/wpa_supplicant.conf
   rfkill unblock wifi
   for filename in /var/lib/systemd/rfkill/*:wlan ; do
       echo 0 > 
   done
fi
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
   /usr/lib/raspberrypi-sys-mods/imager_custom set_keymap 'us'
   /usr/lib/raspberrypi-sys-mods/imager_custom set_timezone 'Europe/Warsaw'
else
   rm -f /etc/localtime
   echo "Europe/Warsaw" >/etc/timezone
   dpkg-reconfigure -f noninteractive tzdata
cat >/etc/default/keyboard <<'KBEOF'
XKBMODEL="pc105"
XKBLAYOUT="us"
XKBVARIANT=""
XKBOPTIONS=""

KBEOF
   dpkg-reconfigure -f noninteractive keyboard-configuration
fi
rm -f /boot/firstrun.sh
sed -i 's| systemd.run.*||g' /boot/cmdline.txt
exit 0

