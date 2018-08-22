// From the example code at www.npmjs.com/package/xbee-api
var util       = require('util');
var SerialPort = require('serialport');
var xbee_api   = require('xbee-api');
var C          = xbee_api.constants;

var xbeeAPI = new xbee_api.XBeeAPI({ // two API modes are available
  api_mode: 1
});

var serialport = new SerialPort("/dev/ttyS0", {
  baudRate: 9600,                    // default baud rate
});

serialport.pipe(xbeeAPI.parser);
xbeeAPI.builder.pipe(serialport);

serialport.on("open", function() {   // uses the serialport module
  var frame_obj = {                  // AT Request to be sent…
    type: C.FRAME_TYPE.AT_COMMAND,   // Prepare for an AT command
    command: "NI",                   // Node identifer command
    commandParameter: [],            // No parameters needed
  };
  xbeeAPI.builder.write(frame_obj);
});

// The data frames received are outputted by this function
xbeeAPI.parser.on("data", function(frame) {
    console.log("Object> ", frame);
});

