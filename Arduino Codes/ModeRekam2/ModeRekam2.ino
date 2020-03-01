#include <SD.h>
#include <SPI.h>
#include <Filters.h>

FilterOnePole low(LOWPASS, 10);
int analogPin = 0;
unsigned long time,a;


char charFileName[13];

File dataFile;
//unsigned long time,a;

const int chipSelect = 10;

void setup()
{
	
  Serial.begin(9600);
  SD.begin(chipSelect);

	generateFiles();
delay(10000);
       
	
}

void loop()
{
  
 if(abs(low.input((analogRead(analogPin))-511))>50)
  {
    
   
	dataFile = SD.open(charFileName, FILE_WRITE);
 // a = millis();       
	for (int i = 0; i<50;i++)
{		
 
		dataFile.print(low.input((analogRead(analogPin))-511));
		dataFile.print(";");
                delay(11);
                
}
//time = millis();
//Serial.println(time-a);

dataFile.println("");
dataFile.close();
  
	}
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
