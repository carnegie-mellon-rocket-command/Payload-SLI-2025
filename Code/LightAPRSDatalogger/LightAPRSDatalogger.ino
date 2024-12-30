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

#include <SPI.h>
#include <SD.h>

const int chipSelect = A1;

int i=0;
int writing=1;

void setup() {
  // Open SerialUSB communications and wait for port to open:
  SerialUSB.begin(9600);
  // wait for SerialUSB Monitor to connect. Needed for native USB port boards only:
  
  //while (!SerialUSB); //THIS LINE STOPS PROGRAM IF SERIAL NOT CONNECTED !!!!!!

  SerialUSB.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    SerialUSB.println("initialization failed. Things to check:");
    SerialUSB.println("1. is a card inserted?");
    SerialUSB.println("2. is your wiring correct?");
    SerialUSB.println("3. did you change the chipSelect pin to match your shield or module?");
    SerialUSB.println("Note: press reset button on the board and reopen this SerialUSB Monitor after fixing your issue!");
    while (true);
  }

  SerialUSB.println("initialization done.");
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
  dataString=String(i);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  

  // if the file is available, write to it:
  if (dataFile && writing) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the SerialUSB port too:
    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("error opening datalog.txt");
  }

  i++;

  if (i==400) {
    dataFile.close();
    writing=0;
  }
}
