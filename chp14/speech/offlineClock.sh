#!/bin/bash
NOW=$(date +"It is %M minutes past %l %p")
echo $NOW
pico2wave -w temporaryTime.wav "$NOW"
lame temporaryTime.wav temporaryTime.mp3
mplayer -ao alsa:device=bluetooth temporaryTime.mp3
rm temporaryTime.*
