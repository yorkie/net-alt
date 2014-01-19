
var netAlt = require('./lib');

for (var i=0; i<900; i++) {
  var socket = new netAlt.Socket(143, '163.177.65.209');
  socket.count = 0;
  socket.connect(function(err, data) {
    console.log(data);
  });
}

var readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
})