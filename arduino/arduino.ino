// button pin
int pinNumber = 7;
// debugger led
int led = 13;
// simple switch variable
bool buttonClicked = false;
bool ledOn = false;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup()
{
  // set the bps
  Serial.begin(57600);
  // set the input pin
  pinMode(pinNumber, INPUT);
  // initialize the LED pin as an output:
  pinMode(led, OUTPUT);
}

void loop()
{
  // read the button state
  int tmpButtonState = digitalRead(pinNumber);

  // if it has not been clicked yet and it's down we can send the message
  if (tmpButtonState == HIGH && !buttonClicked) {
    Serial.println(F("{Hello World}"));
    delay(1000); // wait 1 s before sending the next message
    buttonClicked = true;
  }
  // print the string when a newline arrives:
  if (stringComplete) {
    if (inputString == "{on}") {
      ledOn = true;
    } else if (inputString == "{off}") {
      ledOn = false;
    }
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  // if the button is not pressed anymore we reset the buttonClicked variable to false
  if (tmpButtonState == LOW)
    buttonClicked = false;

  digitalWrite(led, ledOn ? HIGH : LOW);
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
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '}') {
      stringComplete = true;
    }
  }
}
