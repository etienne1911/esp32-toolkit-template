#include "Arduino.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("** Hello world **");
	Serial.println("** Done setup **");
}

void loop()
{

	Serial.println("loop");
	delay(1000);
}