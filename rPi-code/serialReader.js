var serialport = require('serialport');// include the library
   // SerialPort = serialport.SerialPort; // make a local instance of it
   // get port name from the command line:
   portName = '/dev/ttyACM0';

var unirest = require('unirest');

var i=1;

var myPort = new serialport(portName, {
   baudRate: 9600,
// stopBits: 2,
   // look for return and newline at the end of each data packet:
   parser: new serialport.parsers.Readline("\n")
 });

myPort.on('open', showPortOpen);
myPort.on('data', sendSerialData);
myPort.on('close', showPortClose);
myPort.on('error', showError);

var receivedData = '';

function showPortOpen() {
   console.log('port open. Data rate: ' + myPort.baudRate);
}
var x;
function sendSerialData(data) {


   // console.log(data.toString());
receivedData += data.toString();
if (receivedData .indexOf('RESULT ') >= 0 && receivedData .indexOf('END') >= 0) {
	if(receivedData .indexOf('RESULT ') > receivedData .indexOf('END')) {
		receivedData = receivedData.substring(receivedData .indexOf('END')+3);
		console.log('order mismatch');
		return;
	}
         // save the data between 'B' and 'E'
		   sendData = receivedData .substring(receivedData .indexOf('RESULT ') + 7, receivedData .indexOf('END'));
		   receivedData = '';
			console.log("SEND DATA "+ i++);
			console.log(sendData);
			console.log("hello");
			var tmpArr = sendData.split(",");
			console.log(tmpArr);
			if(tmpArr.length === 6) {
				var jsonVal = {
					T: tmpArr[0].substring(2),
					H: tmpArr[1].substring(2),
					L: tmpArr[2].substring(2),
					P: tmpArr[3].substring(2),
					R: tmpArr[4].substring(2),
					X: tmpArr[5].substring(2),
					time: new Date(),
				}
				console.log("hello3");
				unirest.post('https://smarttracker-sidhi.firebaseio.com/logs.json')
				.headers({'Accept': 'application/json', 'Content-Type': 'application/json'})
				.send(jsonVal)
				.end(function (response) {
					console.log("sent successfully to firebase");
					console.log(response.body);
				});
			}
			
	     }
}
 
function showPortClose() {
   console.log('port closed.');
}
 
function showError(error) {
   console.log('Serial port error: ' + error);
}
