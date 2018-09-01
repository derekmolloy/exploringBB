#!/usr/bin/perl
# A small Perl script to set up User LED3 to be turned on or off from 
#  Linux console. Written by Derek Molloy (derekmolloy.ie) for the 
#  book Exploring BeagleBone.

$LED3_PATH = "/sys/class/leds/beaglebone:green:usr3";
$command = $ARGV[0];

# Perl Write to LED3 function, the filename  $_[0] is the first argument
#   and the value to write is the second argument $_[1]
sub writeLED3{
  open(FILE, ">" . $LED3_PATH . $_[0] )
     or die "Could not open the file, $!";
  print FILE $_[1] ;
  close(FILE);
}

sub removeTrigger{
  writeLED3 ( "/trigger", "none");
}

print "Starting the LED Perl Script\n";
# 0 means that there is exactly one argument
if ( $#ARGV != 0 ){
  print "There are no arguments. Usage is:\n";
  print " bashLED Command, where command is one of\n";
  print "   on, off, flash or status e.g. bashLED on\n";
  exit 2;
}
print "The LED Command that was passed is: " . $command . "\n";
if ( $command eq "on" ){
  print "Turning the LED on\n";
  removeTrigger();
  writeLED3 ("/brightness", "1");
}
elsif ( $command eq "off" ){
  print "Turning the LED off\n";
  removeTrigger();
  writeLED3 ("/brightness", "0");
}
elsif ( $command eq "flash" ){
  print "Flashing the LED\n";
  removeTrigger();
  writeLED3( "/trigger", "timer");
  writeLED3( "/delay_on", "50");
  writeLED3( "/delay_off", "50");
}
elsif ( $command eq "status" ){
  open(DATA, "<". $LED3_PATH . "/trigger");
  while(<DATA>){
    print "$_";
  }
  close(DATA);
}
print "End of the LED Perl Script\n";
