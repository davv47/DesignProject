/*ledBlink************************************************
* Blink Led if motor is moving
*****************************************************************/

#include <Wire.h>

int sdaIndex = 8;
int ledPin = 13;

void setup() {     
  pinMode(led, OUTPUT);
  
  Wire.begin(sdaIndex);   
  Wire.onReceive(receiveEvent); 
    
}


void loop() {
  delay(1);
}

void receiveEvent(int howMany){
  int i=0;
  while(Wire.available()){
    int c = Wire.read();
  }
  // If motor not moving set pin to high to turn off LED
  if (c == 0){
    digitalWrite(ledPin, HIGH);
  }
  // Otherwise turn on light
  else{
    digitalWrite(ledPin, LOW);
  }
}
