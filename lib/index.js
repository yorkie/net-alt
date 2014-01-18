
var SimpleNet = require('../build/Release/net.node').SimpleNet;

for (var i=0; i<200; i++) {
  var sn = new SimpleNet();
  sn.connect();
}
console.log('done');

var readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
})