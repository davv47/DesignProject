
#include <Wire.h>

int stepperIndex = 7;
int inByte;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(13, OUTPUT);
}

byte x = 0;

void loop()
{
  if(Serial.available()){
    inByte = Serial.read();
    Wire.beginTransmission(stepperIndex);
    Wire.write(inByte);
    Wire.endTransmission
  }
}
