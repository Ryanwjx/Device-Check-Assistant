#!/bin/sh
echo "Clean Probed Modules"

pkill QDesktop

modprobe -r my_ap3216

modprobe -r mykey

modprobe -r my_icm

modprobe -r myled

modprobe -r g_mass_storage

route del default gw XiaoQiang
pkill wpa_supplicant
ifconfig wlan0 down
modprobe -r 8188eu

echo "Start Probe Modules"

modprobe my_ap3216

modprobe mykey

modprobe my_icm

modprobe myled

modprobe libcomposite

modprobe usb_f_mass_storage

modprobe g_mass_storage file=/dev/mmcblk1p1 removable=1

modprobe 8188eu
route del default gw 192.168.0.1
ifconfig wlan0 up
wpa_supplicant -D wext -c /etc/wpa_supplicant.conf -i wlan0 &
udhcpc -i wlan0  

echo "End Probe Modules"