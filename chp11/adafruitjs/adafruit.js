// This example uses mqtt.js to upload the TMP36 temperature to Adafruit IO
var mqtt      = require('mqtt');   // required module
var fs        = require('fs')

var DEVID     = 'Beagle2';          // the individual device id
var AUTHTOKEN = 'feb8ea6179c64b638b35b93ef87d4b42';  // the private auth token
var PORT      = 1883;              // reserved MQTT port
var BROKER    = 'io.adafruit.com';
var URL       = 'mqtt://' + BROKER + ':' + PORT;
var AUTHMETH  = 'molloyd';         // using username
var client    = mqtt.connect(URL, { clientId: DEVID,
                username: AUTHMETH, password: AUTHTOKEN });
var TOPIC     = 'molloyd/feeds/weather.temperature';
var TEMP      = '/sys/bus/iio/devices/iio:device0/in_voltage0_raw'

console.log(URL);
console.log('Starting the Beagle MQTT Adafruit IO Example');

// Convert ADC value into a temperature
function getTemperature(adc_value) {        // from the TMP36 datasheet
   var cur_voltage = (parseInt(adc_value) * 1.80) / 4096; // Vcc = 1.8V, 12-bit
   var diff_degreesC = (cur_voltage-0.75)/0.01;
   return (25.0 + diff_degreesC);
}

client.on('connect', function() {
   setInterval(function(){
      var tempStr = 'invalid', temp;
      try {
         tempStr = fs.readFileSync(TEMP, 'utf8');
         temp = getTemperature(tempStr).toFixed(4);
      }
      catch (err){
         console.log('Failed to Read the temperature sensor.');
      }
      console.log('Sending Temp: ' + temp.toString() + '°C to Adafruit IO');
      client.publish(TOPIC, temp.toString());
   }, 10000);                            // publish data every ten seconds
});
