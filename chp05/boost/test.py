#!/usr/bin/python
# A Python program that calls C program code
import ebb

print "Start of the Python program"
print ebb.hello("Derek")
val = ebb.integrate(0, 3.14159, 1000000)
print "The integral result is: ", val
print "End of the Python program"

