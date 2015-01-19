// A Simple Example Node.js Webserver Running on Port 5050
// Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
// Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
// ISBN 9781118935125. Please see the file README.md in the repository root 
// directory for copyright and GNU GPLv3 license information.   
//
// Improved with feedback from Jeff Albrecht to identify the adapter IP  
// address of the server in the console log.
// Please install the IP node module by typing: npm install ip

var http = require('http');  // require the http module
var ip = require("ip");
var myip = ip.address();
var server = http.createServer(
    function (req, res) {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end('Hello from the BeagleBone Black!\n');
    });
server.listen(5050);
console.log('BBB Web Server running at http://'+myip+':5050/');

