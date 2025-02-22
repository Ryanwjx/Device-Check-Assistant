#!/bin/sh
status=$1
if [ "$status" = "1" ]
then
echo "beep has been open"
echo 255 > /sys/class/leds/beep/brightness
fi

if [ "$status" = "0" ]
then
echo "beep has been close"
echo 0 > /sys/class/leds/beep/brightness
fi