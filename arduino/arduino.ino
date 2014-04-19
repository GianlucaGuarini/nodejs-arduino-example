// pin of the button connected to the arduino board
const int buttonPin = 7;
// pin of the led light
// it's actually also the default arduino debug light
const int ledPin = 13;

// simple variables used to store the led and the button statuses
String buttonStatus = "off";
String ledStatus = "off";

// it will hold all the messages coming from the nodejs server
String inputString = "";

// whether the string received form nodejs is complete
// a string is considered complete by the carriage return '\r'
boolean stringComplete = false;

/**
 *
 * arduino board setup
 *
 */

void setup()
{
  // set the Baud Rate
  Serial.begin(115200);
  // set the input pin
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

/**
 *
 * Default arduino loop function
 * it runs over and over again
 *
 */

void loop()
{
  // read the button state and send a message to the nodejs server
  listenButtonChanges(digitalRead(buttonPin));
  // update the status of the led light connected to the arduino board
  updateLedStatus();
}

/**
 *
 * check the button connected to the pin 7 of the arduino board and
 * send messages through the serial port whenever the button gets clicked
 * @param  { boolean } tmpButtonStatus: its the current status of the button
 *
 */

void listenButtonChanges (boolean tmpButtonStatus) {
  // if it has not been clicked yet and it's down we can send the message
  if (tmpButtonStatus && buttonStatus == "off") {
    buttonStatus = "on";
    Serial.println("Hello World");
    // wait 1 s before sending the next message
    delay(1000);
  }
  // if the button is not pressed anymore we reset the buttonStatus variable to off
  if (!tmpButtonStatus) {
    buttonStatus = "off";
  }
}

/**
 *
 * Update the status of the led light reading the messages dispatched by the nodejs server
 *
 */

void updateLedStatus() {
  // detect whether the string has been completely received
  if (stringComplete) {
    // set and store the current led status
    if (inputString == "on\r") {
      ledStatus = "on";
    }
    if (inputString == "off\r") {
      ledStatus = "off";
    }
    // send the light status to the nodejs server
    Serial.println(ledStatus);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  // turn on or off the led according to the latest light status stored
  digitalWrite(ledPin, ledStatus == "on" ? HIGH : LOW);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a "\n" we detect the end of the string
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}
