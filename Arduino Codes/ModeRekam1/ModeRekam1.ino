#include <SD.h>
#include <SPI.h>
#include <Filters.h>

FilterOnePole low(LOWPASS, 10);
int analogPin = 0;



char charFileName[13];

File dataFile;
unsigned long time,a;

const int chipSelect = 10;

void setup()
{
	
  //Serial.begin(9600);
  SD.begin(chipSelect);

	generateFiles();
       delay(5000);
	
}

void loop()
{
  
  //if ((analogRead(analogPin))-511 > 30)
  //{
	dataFile = SD.open(charFileName, FILE_WRITE);

			
		dataFile.println(low.input((analogRead(analogPin))-511));
		dataFile.close();
   //time = millis();
   //Serial.println(average);
//}	
   // dataFile.println();
	}

void generateFiles() {
	String fileName = String();
	
	unsigned int filenumber = 1;
	while (!filenumber == 0) {
		fileName = "log";
		fileName += filenumber;
		fileName += ".csv";
	
		charFileName[fileName.length() + 1];
		fileName.toCharArray(charFileName, sizeof(charFileName));

		if (SD.exists(charFileName)) {
			filenumber++;
			
		}
		else {
			dataFile = SD.open(charFileName, FILE_WRITE);
			dataFile.close(); 
			
			filenumber = 0;
			
		}
	}
}
