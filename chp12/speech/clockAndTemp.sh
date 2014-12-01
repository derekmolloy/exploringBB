#!/bin/bash
TEMP=$(./tmp36raw)
NOW=$(date +" It is %M minutes past %l %p and it is $TEMP degrees Celsius")
echo $NOW
mplayer -ao alsa:device=bluetooth "http://translate.google.com/translate_tts?tl=en&q=$NOW"
