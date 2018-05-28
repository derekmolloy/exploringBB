#!/bin/bash
# Short script to toggle a GPIO pin at the highest frequency
#  possible using Bash - by Derek Molloy
echo "out" > /sys/class/gpio/gpio60/direction
COUNTER=0
while [ $COUNTER -lt 100000 ]; do
    echo 0 > /sys/class/gpio/gpio60/value
    echo 1 > /sys/class/gpio/gpio60/value
    let COUNTER=COUNTER+1
done
