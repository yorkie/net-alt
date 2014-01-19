
var SimpleNet = require('../build/Release/net.node').SimpleNet;

for (var i=0; i<200; i++) {
  var socket = new SimpleNet(143, '163.177.65.209');
  socket.count = 0;
  socket.connect(function(err, data) {
    console.log(data);
    if (this.count++ < 1) {
      this.write('a1 login l900422@vip.qq.com Lyz15285115151\r\n');
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