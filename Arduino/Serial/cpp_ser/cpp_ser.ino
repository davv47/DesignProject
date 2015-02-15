#include <Wire.h>

int ledPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}


void loop(){
  if (Serial.available()>0){
    int serInByte = Serial.read();
    if (1) {
      digitalWrite(ledPin, HIGH);
      delay(1000*serInByte);
      digitalWrite(ledPin,LOW);
      delay(1000);
    } 
  }
}

