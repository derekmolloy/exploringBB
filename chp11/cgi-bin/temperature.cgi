#!/bin/bash
printf "Content-type: text/html\n\n"
printf "<html><head>"
printf "<meta charset=\"UTF-8\">"
printf "<title>Beagle Board Temperature</title></head>"
printf "<body><h1>Beagle Board Temperature</h1><para>"
printf "The temperature in the room is "
/usr/local/bin/tmp36raw
printf " degrees Celsius</para></html>"

