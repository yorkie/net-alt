
var netAlt = require('./lib');

for (var i=0; i<200; i++) {
  var socket = new netAlt.Socket(143, '163.177.65.209');
  socket.count = 0;
  socket.connect(function(err, data) {
    console.log(data);
    if (this.count++ < 1) {
      this.write('a1 login l900422@vip.qq.com xxxxxxxxxx\r\n');
    } else {
      this.end();
    }
  });
}

var readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
})