var net = require('net')
var readline = require('readline');

for (var i=0; i<200; i++) {
  net.connect(143, "imap.qq.com", function() {
    console.log('connected');
  });
}

readline.createInterface({
  input: process.stdin,
  output: process.stdout
})