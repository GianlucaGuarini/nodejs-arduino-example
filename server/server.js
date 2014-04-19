var app = require('http').createServer(handler),
  io = require('socket.io').listen(app),
  fs = require('fs'),
  url = require('url'),
  SerialPort = require('serialport').SerialPort,
  // initialize serialport using the /dev/cu.usbmodem1411 serial port
  // remember to change this string if your arduino is using a different serial port
  sp = new SerialPort('/dev/cu.usbmodem1411', {
    baudRate: 115200
  }),
  // this var will contain the message string dispatched by arduino
  arduinoMessage = '',
  /**
   * helper function to load any app file required by client.html
   * @param  { String } pathname: path of the file requested to the nodejs server
   * @param  { Object } res: http://nodejs.org/api/http.html#http_class_http_serverresponse
   */
  readFile = function(pathname, res) {
    // an empty path returns client.html
    if (pathname === '/')
      pathname = 'client.html';

    fs.readFile('client/' + pathname, function(err, data) {
      if (err) {
        console.log(err);
        res.writeHead(500);
        return res.end('Error loading client.html');
      }
      res.writeHead(200);
      res.end(data);
    });
  },
  /**
   *
   * This function is used as proxy to print the arduino messages into the nodejs console and on the page
   * @param  { Buffer } buffer: buffer data sent via serialport
   * @param  { Object } socket: it's the socket.io instance managing the connections with the client.html page
   *
   */
  sendMessage = function(buffer, socket) {
    // concatenating the string buffers sent via usb port
    arduinoMessage += buffer.toString();

    // detecting the end of the string
    if (arduinoMessage.indexOf('\r') >= 0) {
      // log the message into the terminal
      // console.log(arduinoMessage);
      // send the message to the client
      socket.volatile.emit('notification', arduinoMessage);
      // reset the output string to an empty value
      arduinoMessage = '';
    }
  };

// creating a new websocket
io.sockets.on('connection', function(socket) {
  // listen all the serial port messages sent from arduino and passing them to the proxy function sendMessage
  sp.on('data', function(data) {
    sendMessage(data, socket);
  });
  // listen all the websocket "lightStatus" messages coming from the client.html page
  socket.on('lightStatus', function(lightStatus) {
    sp.write(lightStatus + '\r', function() {
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

// L3T'S R0CK!!!
// creating the server ( localhost:8000 )
app.listen(8000);
// server handler
function handler(req, res) {
  readFile(url.parse(req.url).pathname, res);
}