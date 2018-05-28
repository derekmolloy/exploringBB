var b      = require('bonescript');
var led    = "P2_08";          // GPIO1_28 (GPIO 60)
var button = "P2_22";          // GPIO1_14 (GPIO 46)
var isOn   = false;            // isOn is a Boolean flag

console.log('Setting up the inputs and outputs');
b.pinMode(button, b.INPUT, 7, 'pullup', 'fast');
b.pinMode(led, b.OUTPUT);

console.log('Flashing the LED on GPIO 60');
timer = setInterval(toggleLED, 500); // each half second call toggleLED()
setTimeout(stopTimer, 5000);         // stop after 5 seconds

console.log('Reading the button on GPIO 46');
b.digitalRead(button, display);

console.log('End of the application');


function toggleLED(){
   isOn = !isOn;               // invert the isOn state on each call
   if (isOn) b.digitalWrite(led, 1);  // light the LED
   else b.digitalWrite(led, 0);       // turn off the LED
};

function stopTimer(){
   clearInterval(timer);
};

function display(x) {
    console.log('Button value = ' + x.value);
};
