
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(9, 8); // RX, TX
Servo mine;

//const int sensitivity = 200;
int classi;

void setup()
{
	Serial.begin(9600);
	mySerial.begin(9600);
//mySerial.onReceive(datareceived);
//Mouse.begin();
mine.attach(10);
mine.write(90);
}

void loop()
{
	int classi = mySerial.read();

//Serial.println(classi);
	switch (classi)
	{
	case 1:
		mine.write(130);
Serial.println("up");
delay(500);
		break;
	case 2:
		mine.write(40);
Serial.println("down");
delay(500);
		break;
	case 3:
		mine.write(130);
Serial.println("right");
delay(500);
		break;
	case 4:
		mine.write(60);
Serial.println("left");
delay(500);
		break;
	default:
		mine.write(90);
delay(500);
		break;
	}
}

