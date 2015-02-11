// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int I2C_ADDRESS = 7;

void setup() {
  Wire.begin(I2C_ADDRESS);

  box.x1 = box.x2 = box.y1 = box.y2 = 0;

  Wire.onRequest(handleI2CRequest);
  Wire.onReceive(handleI2CReceive);
}


void loop()
{
  delay(1);
}

void handleI2CReceive(int numBytes)
{
  char command = Wire.receive(); // pretty much just ignore the command

  return;
}

void handleI2CRequest()
{
  byte data[4];

  // the code below just sends
  // data from the global variable
  // box, a struct with 4 char members
  //
  data[0] = box.x1;
  data[1] = box.y1;
  data[2] = box.x2;
  data[3] = box.y2;
  
  Wire.send(data, 4);
  
  return;
}

