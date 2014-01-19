
var SimpleNet = require('../build/Release/net.node').SimpleNet;

for (var i=0; i<1; i++) {
  var socket = new SimpleNet(143, '163.177.65.209');
  socket.connect(function() {
    // TODO
  });
}

var readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
})