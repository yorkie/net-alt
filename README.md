
# SimpleNet

Alternative module to net, It's under being developed.


### Installation

```bash
$ npm install net-alt
```


### Usage

```js
var socket = new netAlt.SimpleNet(143, '163.177.65.209');
socket.connect(function(err, data) {
  this.write('client says hello!\r\n');
  this.end(); // close fd via this.
});
```


### Current comparison with 200 connections

* net(NodeJS): 8.8M

* SimpleNet: 7.2M


### LICENSE

MIT