#!/bin/bash
mplayer -ao alsa:device=bluetooth "http://translate.google.com/translate_tts?tl=en&q=$*"
