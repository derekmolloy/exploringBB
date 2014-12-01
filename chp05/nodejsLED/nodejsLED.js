// Simple Node.js example program to set up User LED3 to be turned on or off from
//  the Linux console. 
// Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
// Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
// ISBN 9781118935125. Please see the file README.md in the repository root 
// directory for copyright and GNU GPLv3 license information. 

// Ignore the first two arguments (nodejs and the program name)
var myArgs = process.argv.slice(2);
var LED3_PATH = "/sys/class/leds/beaglebone:green:usr3"

function writeLED( filename, value, path ){
  var fs = require('fs');
  // This call must be syncronous! Otherwise the timer will not work as there are
  //  three calls to write that happen at the same time for the flash call
  try {
     fs.writeFileSync(path+filename, value); 
  }
  catch (err) {
     console.log("The Write Failed to the File: " + path+filename);
  }
}

function removeTrigger(){
   writeLED("/trigger", "none", LED3_PATH);
}

console.log("Starting the LED Node.js Program");
if (myArgs[0]==null){
   console.log("There is an incorrect number of arguments.");
   console.log("  Usage is: nodejs nodejsLED.js command");
   console.log("  where command is one of: on, off, flash or status.");
   process.exit(2);   //exits with the error code 2 (incorrect usage)
}
switch (myArgs[0]) {
   case 'on':
      console.log("Turning the LED On");
      removeTrigger();
      writeLED("/brightness", "1", LED3_PATH);
      break;
   case 'off':
      console.log("Turning the LED Off");
      removeTrigger();
      writeLED("/brightness", "0", LED3_PATH);
      break;
   case 'flash':
      console.log("Making the LED Flash");
      writeLED("/trigger", "timer", LED3_PATH);
      writeLED("/delay_on", "50", LED3_PATH);
      writeLED("/delay_off", "50", LED3_PATH);  
      break;
   case 'status':
      console.log("Getting the LED Status");
      fs = require('fs');
      fs.readFile(LED3_PATH+"/trigger", 'utf8', function (err, data) {
         if (err) { 
            return console.log(err);
         }
         console.log(data);
      });
      break;
   default:
      console.log("Invalid Command");
}
console.log("End of Node.js script");
