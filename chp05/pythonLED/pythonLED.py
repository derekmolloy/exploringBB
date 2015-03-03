#!/usr/bin/python 
# A small Python program to set up User LED3 to be turned on or off from
#  the Linux console. 
# Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
# Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
# ISBN 9781118935125. Please see the file README.md in the repository root 
# directory for copyright and GNU GPLv3 license information.      

import sys
LED3_PATH = "/sys/class/leds/beaglebone:green:usr3"

def writeLED ( filename, value, path=LED3_PATH ):
   "This function writes the value passed to the file in the path"
   fo = open( path + filename,"w")  
   fo.write(value)
   fo.close()
   return

def removeTrigger():
   writeLED (filename="/trigger", value="none")
   return

print "Starting the LED Python Script"
if len(sys.argv)!=2:
   print "There are an incorrect number of arguments"
   print "  usage is:  pythonLED.py command"
   print "  where command is one of on, off, flash or status."
   sys.exit(2)
if sys.argv[1]=="on":
   print "Turning the LED on"
   removeTrigger()
   writeLED (filename="/brightness", value="1")
elif sys.argv[1]=="off":
   print "Turning the LED off"
   removeTrigger()
   writeLED (filename="/brightness", value="0")
elif sys.argv[1]=="flash":
   print "Flashing the LED"
   writeLED (filename="/trigger", value="timer")
   writeLED (filename="/delay_on", value="50")
   writeLED (filename="/delay_off", value="50")
elif sys.argv[1]=="status":
   print "Getting the LED trigger status"
   fo = open( LED3_PATH + "/trigger", "r")
   print fo.read()
   fo.close()
else:
   print "Invalid Command!"
print "End of Python Script"
