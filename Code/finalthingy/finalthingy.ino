
/*
  NEEDS Adafruit SPIflash library
  
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library. Pin numbers reflect the default
  SPI pins for Uno and Nano models

  The circuit:
   analog sensors on analog pins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 Pin 10 used here for consistency with other Arduino examples
    (for MKR Zero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified  24 July 2020
  by Tom Igoe

  This example code is in the public domain.

*/
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>


#define BattPin       A5

const int chipSelect = A1;
const bool writing = true;

const int launchVelocityThreshold = 20;
const int launchAltitudeThreshold = 40;

bool launched = false;
int launchTime;

int startTime = 0;
float startAltitude;

int i=0;
int notPrinted = 1;
int fileNum = 0;
float altitudes[2];
float times[2];

float maxVelocity = 0;
float maxAltitude = 0;

Adafruit_BMP085 bmp;



void setup() {
    pinMode(BattPin, INPUT);
  // Open SerialUSB communications and wait for port to open:
  
  SerialUSB.begin(9600);
  // wait for SerialUSB Monitor to connect. Needed for native USB port boards only:
  
  while (!SerialUSB); //THIS LINE STOPS PROGRAM IF SERIAL NOT CONNECTED !!!!!!
  
  SerialUSB.print("Initializing SD card...");
  bmp.begin();
  if (!SD.begin(chipSelect)) {
    SerialUSB.println("initialization failed. Things to check:");
    SerialUSB.println("1. is a card inserted?");
    SerialUSB.println("2. is your wiring correct?");
    SerialUSB.println("3. did you change the chipSelect pin to match your shield or module?");
    SerialUSB.println("Note: press reset button on the board and reopen this SerialUSB Monitor after fixing your issue!");
    while (true);
  }

  SerialUSB.println("initialization done.");
  



  for (int i = 0; i < 1000; i ++){
    if (SD.exists("log" + String(i) + ".txt")) continue;
    else {
      if (writing) {
        fileNum = i;
      }
      else {
        fileNum = i-1;
      }
      break;
    }
  }
  startTime = millis();
  launchTime = startTime;
  startAltitude = bmp.readAltitude();
  times[1] = startTime;
  SerialUSB.println("Setup completed. Using fileNum: " + String(fileNum) + " and startTime: " + String(startTime));
}

float readBatt() {


     
  float value =analogRead(BattPin);
  value +=analogRead(BattPin);
  value +=analogRead(BattPin);
  value = value / 3.0f;
  value = 10 * (value * 1.65) / 1024.0f;

  return value ;
  
}


void loop() {
  // make a string for assembling the data to log:
  
  String dataString = "";

  // read three sensors and append to the string:
  /*
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }
  */
  float altitude = bmp.readAltitude();
  float tempC = bmp.readTemperature();
  altitude += bmp.readAltitude();
  float pressure = bmp.readPressure();
  altitude += bmp.readAltitude();
  altitude /= 3;
  altitudes[0] = altitudes[1];
  altitudes[1] = altitude;
  times[0] = times[1];
  times[1] = millis() - startTime;
  float velocity =1000 *(altitudes[1] - altitudes[0]) / (times[1] - times[0]);
  if (abs(velocity) > maxVelocity && altitudes[0] != 0) {
    maxVelocity = abs(velocity);
    if (!launched && velocity > launchVelocityThreshold ){
      launched = true;
      launchTime = times[1];
    }
  }
  maxAltitude = max(altitude,maxAltitude);

  dataString=String(times[1]) + " " + 
             String(startTime == launchTime ? 0 : (times[1] + startTime - launchTime)) + " " + 
             String(tempC) + " " + 
             String(pressure) + " " + 
             String(altitude) + " " + 
             String(velocity) + " " +
             String(maxVelocity) + " " +
             String(maxAltitude) + " " +
             String(readBatt()); 

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  if (writing){
    File dataFile = SD.open("log" + String(fileNum) + ".txt", FILE_WRITE);
    

  
    dataFile.println(dataString);
    dataFile.close();
    SerialUSB.println(dataString);
  }
  else{
    if (notPrinted){
      File dataFile = SD.open("log" + String(fileNum) + ".txt", FILE_WRITE);
      if (dataFile){
        while (dataFile.available()){
          SerialUSB.write(dataFile.read());
        }
      }
      dataFile.close();
      notPrinted = 0;
    }
  }
  // print to the SerialUSB port too:
  
  


}
