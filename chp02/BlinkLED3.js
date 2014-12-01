// Example application for flashing an LED using BoneScript
// Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
// Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
// ISBN 9781118935125. Please see the file README.md in the repository root 
// directory for copyright and GNU GPLv3 license information.   

var b = require('bonescript'); //using BoneScript
var LED3Pin = "USR3";          //USR3 is D5 on the BBB

b.pinMode(LED3Pin, b.OUTPUT);  //set up LED3Pin as an output
var isOn = false;              //isOn will be a Boolean flag
setInterval(toggleLED, 500);   //each half second call toggleLED()

function toggleLED() {
    isOn = !isOn;              //invert the isOn state on each call
    if (isOn) b.digitalWrite(LED3Pin, 1);  //light the LED
    else b.digitalWrite(LED3Pin, 0);       //turn off the LED
    console.log('LED On is: ' + isOn);     //output the state
}
