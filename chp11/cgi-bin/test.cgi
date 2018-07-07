#!/bin/bash
printf "Content-type: text/html\n\n"
printf "<html><head>"
printf "<meta charset=\"UTF-8\">"
printf "<title>Hello Beagle Board</title></head>"
printf "<body><h1>Hello Beagle Board</h1><para>"
hostname
printf " has been up "
uptime
printf "</para></html>"

