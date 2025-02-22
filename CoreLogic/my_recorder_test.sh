#!/bin/sh
status=$1
if [ "$status" = "1" ]
then
echo "recorder has been open"
pkill arecord
pkill aplay
./device_test/line_in_config.sh
arecord -f cd -d 0 record.wav
fi

if [ "$status" = "0" ]
then
echo "recorder has been close"
pkill arecord
pkill aplay
aplay record.wav
fi