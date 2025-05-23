#!/bin/sh
status=$1
if [ "$status" = "1" ]
then
echo "speaker has been open"
pkill aplay
aplay /usr/share/sounds/alsa/Front_Right.wav
fi

if [ "$status" = "0" ]
then
echo "speaker has been close"
pkill aplay
fi