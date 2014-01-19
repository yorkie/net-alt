var net = require('net')
var readline = require('readline');

for (var i=0; i<900; i++) {
  net.connect(143, "imap.qq.com")
    .on('data', function(chunk) {
      console.log(chunk.toString());
    })
}

readline.createInterface({
  input: process.stdin,
  output: process.stdout
})