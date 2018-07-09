var express = require('express');
var app = express();

app.get('/', function (req, res) {
  res.send('Hello from the Beagle board!');
});

var server = app.listen(5050, function () {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Application listening at http://%s:%s', host, port);
});
