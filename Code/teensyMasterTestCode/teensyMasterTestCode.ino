/*
Modified from the demo that Dronebotworkshop created.
I2C Master Demo
i2c-master-demo.ino
Demonstrate use of I2C bus
Master sends character and gets reply from Slave
DroneBot Workshop 2019
https://dronebotworkshop.com
*/

// Include Arduino Wire library for I2C
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

// Define Slave answer size
#define ANSWERSIZE 5

void setup() {

// Initialize I2C communications as Master
Wire.begin();

// Setup serial monitor
SerialUSB.begin(9600);
SerialUSB.println("I2C Master Demonstration");
}

byte i = 0;

void loop() {
delay(1000);
SerialUSB.println("\nWrite data to slave");

// Write a character to the Slave
Wire.beginTransmission(SLAVE_ADDR);
Wire.write(i);
Wire.endTransmission();
i += 1;

SerialUSB.print("One byte sent to slave = ");
SerialUSB.println(i, HEX);

// Read response from Slave
// Read back 5 characters
int number_of_bytes_returned = Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);

// Add characters to string
String response = "";
while (Wire.available()) {
char b = Wire.read();
response += b;
}

// Print to Serial Monitor
SerialUSB.print(number_of_bytes_returned);
SerialUSB.println(" bytes returned from slave = " + response);
}