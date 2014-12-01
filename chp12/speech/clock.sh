#!/bin/bash
NOW=$(date +" It is %M minutes past %l %p ")
echo $NOW
mplayer -ao alsa:device=bluetooth "http://translate.google.com/translate_tts?tl=en&q=$NOW"
