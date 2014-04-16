// button pin
int pinNumber = 7;
// debugger led
int led = 13;
// simple switch variable
bool buttonClicked = false;

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
  if (tmpButtonState == HIGH && buttonClicked == false) {
    Serial.printLn("Hello World");
    digitalWrite(led, HIGH);
    delay(1000); // wait 1 s before sending the next message
    buttonClicked = true;
  } else {
    // just for debugging
    digitalWrite(led, LOW);
  }
  // if the button is not pressed anymore we reset the buttonClicked variable to false
  if (tmpButtonState == LOW)
    buttonClicked = false;
}
