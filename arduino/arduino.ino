// button pin
int pinNumber = 7;
// debugger led
int led = 13;
// simple switch variable
bool buttonClicked = false;

void setup()
{
  // set the bps
	Serial.begin(9600);
  // set the input pin
	pinMode(pinNumber, INPUT);
}
 
void loop()
{
	// read the button state
	int tmpButtonState = digitalRead(pinNumber);
	// if it has not been clicked yet and it's down we can send the message
	if (tmpButtonState == HIGH && buttonClicked == false) {
		Serial.print("ciao");
		buttonClicked = true;
    delay(500); // wait 500 ms before sending the next message
	} 
	// if the button is not pressed anymore we reset the buttonClicked variable to false
	if (tmpButtonState == LOW) 
		buttonClicked = false;
}