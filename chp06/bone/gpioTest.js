var b      = require('bonescript');
var led    = "P2_08";          // GPIO1_28 (GPIO 60)
var button = "P2_22";          // GPIO1_14 (GPIO 46)
var isOn   = false;            // isOn is a Boolean flag

console.log('Testing the inputs output response');
b.getPlatform(displayPlatform);
b.pinMode(button, b.INPUT, 7, 'pullup', 'fast');
b.pinMode(led, b.OUTPUT);

console.log('Attaching interrupt to GPIO 46');
b.attachInterrupt(button, true, b.CHANGE, buttonPressed);
console.log('End of the application');

function displayPlatform(platform){
   console.log('Platform name is ' + platform.name);
};

function buttonPressed(){
   isOn = !isOn;              // invert the isOn state on each call
   if (isOn) b.digitalWrite(led, 1);  // light the LED
   else b.digitalWrite(led, 0);       // turn off the LED
};
