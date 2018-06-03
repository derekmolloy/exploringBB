var b = require('bonescript');
var adc = "P2.35";   // AIN0 P1.19  AIN1 P1.21  AIN2 P1.23
                     // AIN3 P1.25  AIN4 P1.27  AIN5 P2.35 (3.3)
                     // AIN6 P1.02 (3.3)  AIN7 P2.36
var sampleTime = 1000;
var endTime = 10000;

console.log('Reading AIN on '+adc+' every '+sampleTime+'ms');
timer = setInterval(readAIN, sampleTime);
setTimeout(stopTimer, endTime);

function readAIN() {
    value = b.analogRead(adc);
    console.log('ADC Value = ' + value);
};

function stopTimer(){
   clearInterval(timer);
};


