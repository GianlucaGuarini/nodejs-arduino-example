/**
 * 
 * Public variables
 * 
 */

// load serialport
var SerialPort = require('serialport').SerialPort,
	// initialize serialport using the /dev/ttyACM0 serial port
	// remember to change this string if your arduino is using a different serial port
	sp = new SerialPort('/dev/ttyACM0', {
		baudrate: 9600
	}),
	// this var will contain the output string coming from arduino
	outputStr = '',
	// timer used to output correctly all the messages coming from arduino
	outputTimer;

/**
 * 
 * This function is used as proxy to print the arduino messages into the nodejs console
 * @param  { Buffer } buffer: buffer data sent via serialport
 * 
 */

var printOutput = function (buffer) {
	// concatenating the string buffers sent via usb port
	outputStr += buffer.toString();

	// avoiding to trigger the timer more than once
	clearTimeout(outputTimer);

	// wait 500 ms and then output the message
	outputTimer = setTimeout(function() {
		console.log(outputStr);
		// reset the output string to an empty value
		outputStr = '';
	}, 500);
};

// listen all the serial port messages sent from arduino and passing them to the proxy function printOutput
sp.on('data', function(data) {
	printOutput(data);
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