#!/bin/bash
TEMP=$(./tmp36raw)
NOW=$(date +"It is %M minutes past %l %p and it is $TEMP degrees Celsius")
echo $NOW
pico2wave -w temporaryTime.wav "$NOW"
lame temporaryTime.wav temporaryTime.mp3
mplayer -ao alsa:device=bluetooth temporaryTime.mp3
rm temporaryTime.*
