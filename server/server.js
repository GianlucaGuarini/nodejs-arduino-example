/**
 *
 * Public variables
 *
 */

// load serialport
var app = require('http').createServer(handler),
  io = require('socket.io').listen(app),
  fs = require('fs'),
  SerialPort = require('serialport').SerialPort,
  // initialize serialport using the /dev/cu.usbmodem1411 serial port
  // remember to change this string if your arduino is using a different serial port
  sp = new SerialPort('/dev/cu.usbmodem1411', {
    baudRate: 57600
  }),
  // this var will contain the output string coming from arduino
  outputStr = '';

// creating the server ( localhost:8000 )
app.listen(8000);

// on server started we can load our client.html page
function handler(req, res) {
  fs.readFile('client/client.html', function(err, data) {
    if (err) {
      console.log(err);
      res.writeHead(500);
      return res.end('Error loading client.html');
    }
    res.writeHead(200);
    res.end(data);
  });
}
/**
 *
 * This function is used as proxy to print the arduino messages into the nodejs console
 * @param  { Buffer } buffer: buffer data sent via serialport
 *
 */

var printOutput = function(buffer, socket) {
  // concatenating the string buffers sent via usb port
  outputStr += buffer.toString();

  if (outputStr.indexOf('{') >= 0 && outputStr.indexOf('}') >= 0) {
    // log the message into the terminal
    console.log(outputStr);
    // send the message to the client
    socket.volatile.emit('notification', outputStr.replace(/\{|\}/gi, ''));
    // reset the output string to an empty value
    outputStr = '';
  }
};

// creating a new websocket
io.sockets.on('connection', function(socket) {
  // listen all the serial port messages sent from arduino and passing them to the proxy function printOutput
  sp.on('data', function(data) {
    printOutput(data, socket);
  });
  socket.on('lightStatus', function(lightStatus) {
    sp.write('{' + lightStatus + '}', function() {
      // log the light status into the terminal
      console.log('the light should be: ' + lightStatus);
    });
  });
});

// just some debug listeners
sp.on('close', function(err) {
  console.log('Port closed!');
});

sp.on('error', function(err) {
  console.error('error', err);
});

sp.on('open', function() {
  console.log('Port opened!');
});