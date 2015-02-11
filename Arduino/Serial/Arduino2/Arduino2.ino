// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int sdaIndex = 7;
int SDA_Pin = 2;
int SCL_Pin = 3;

void setup()
{
  Wire.begin(sdaIndex);           // join i2c bus with address
  
  //Disable Internal Pullup Resistors
  pinMode(SDA_Pin, INPUT);
  pinMode(SCL_Pin, INPUT);
  
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(13, OUTPUT);
}

void loop()
{
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
  digitalWrite(13, HIGH);
  delay(1000);
  while(Wire.available()) // loop through all b
  {
    byte c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }  
  digitalWrite(13, LOW);
  delay(1000);
}
