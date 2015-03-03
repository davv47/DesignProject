/*ledBlink************************************************
* Blink Led if motor is moving
*****************************************************************/

#include <Wire.h>

int sdaIndex = 8;
int ledPin = 13;
int serInByte[4];

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Wire.begin(sdaIndex);   
  Wire.onReceive(receiveEvent); 
    
}
void loop() {
  delay(1);
}

void receiveEvent(int howMany){
  Serial.println("got msg: ");
  int i=0;
  while(Wire.available()){
    serInByte[i] = Wire.read();
    i++;
  }
  // If motor not moving set pin to high to turn off LED
  Serial.println(serInByte[0]);
  if (serInByte[0] == 0){
    digitalWrite(ledPin, HIGH);
  }
  // Otherwise turn on light
  else{
    digitalWrite(ledPin, LOW);
  }
}
