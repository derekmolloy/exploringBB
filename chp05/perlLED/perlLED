#!/usr/bin/perl
# A small Perl script to set up User LED3 to be turned on or off from 
#  Linux console. Written by Derek Molloy (derekmolloy.ie) for the 
#  book Exploring BeagleBone.

$LED3_PATH = "/sys/class/leds/beaglebone:green:usr3";
$command = $ARGV[0];

# Example Perl function
sub removeTrigger{
  open(FILE, ">" . $LED3_PATH . "/trigger")
     or die "Could not open the trigger file, $!";
  print FILE "none";
  close(FILE);
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
  open(FILE, ">" . $LED3_PATH . "/brightness")
     or die "Could not open the brightness file, $!";
  print FILE "1";
  close(FILE);
}
elsif ( $command eq "off" ){
  print "Turning the LED off\n";
  removeTrigger();
  open(FILE, ">" . $LED3_PATH . "/brightness")
     or die "Could not open the brightness file, $!";
  print FILE "0";
  close(FILE);
}
elsif ( $command eq "flash" ){
  print "Flashing the LED\n";
  removeTrigger();
  open(FILE, ">" . $LED3_PATH . "/trigger")
     or die "Could not open the trigger file, $!";
  print FILE "timer";
  close(FILE);
  open(FILE, ">" . $LED3_PATH . "/delay_on")
     or die "Could not open the delay_on file, $!";
  print FILE "50";
  close(FILE);
  open(FILE, ">" . $LED3_PATH . "/delay_off")
     or die "Could not open the delay_off file, $!";
  print FILE "50";
  close(FILE);
}
elsif ( $command eq "status" ){
  open(DATA, "<". $LED3_PATH . "/trigger");
  while(<DATA>){
    print "$_";
  }
  close(DATA);
}
print "End of the LED Perl Script\n";
