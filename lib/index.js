
var SimpleNet = require('../build/Release/net.node').SimpleNet;

for (var i=0; i<200; i++) {
  var socket = new SimpleNet(143, '163.177.65.209');
  socket.connect(function(err, chunk) {
    console.log(chunk);
  });
}
console.log('done!');

var readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
})