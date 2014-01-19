
# net-alt

Alternate implementation to replace the partial core net, It's under being developed.


### Features

* Less RSS

* Network client within a large number of concurrent connections


### Installation

```bash
$ npm install net-alt
```


### Usage

```js
var socket = new netAlt.Socket(143, '163.177.65.209');
socket.connect(function(err, data) {
  this.write('client says hello!\r\n');
  this.end(); // close fd via this.
});
```


### Current comparison with 900 connections

* net(NodeJS): 14.0~M

* SimpleNet: 8.3M


### LICENSE

MIT