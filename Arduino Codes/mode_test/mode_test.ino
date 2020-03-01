#include <Filters.h>

FilterOnePole low(LOWPASS, 10);
int analogPin = 0;

float Xn;


const int chipSelect = 10;

void setup()
{
	Serial.begin(9600);
	
  /* add setup code here */

}

void loop()
{
	//Xn = (analogRead(analogPin))-510;
low.input(analogRead(analogPin)-511);
	Serial.println(low.output());
  /* add main program code here */
//delayMicroseconds(800);
}


