var b = require('bonescript');
var pin = "P2_01";   // Use P1.08 or P1.36 by default on PocketBeagle
var dutyCycle = 0.75;
var frequency = 10000;

b.pinMode(pin, b.OUTPUT);
b.getPinMode(pin, printPinMux);
b.analogWrite(pin, dutyCycle, frequency, display);

function printPinMux(val){
   console.log('mux = '+val.mux);
   console.log('name = '+val.name);
}

function display(val){
   console.log(val);
}
