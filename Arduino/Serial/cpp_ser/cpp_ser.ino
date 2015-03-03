#include <Wire.h>

int ledPin = 13;
const int sdaMotorIndex = 8;
const int sdaLEDIndex = 9;
const int SDA_Pin = 2;
const int SCL_Pin = 3;

int serInByte[4];

void setup() {
  Serial.begin(9600);
  Wire.begin(sdaMotorIndex);
  //pinMode(SDA_Pin, INPUT);
  //pinMode(SCL_Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  
}

void loop(){
  if (Serial.available()>0){
    int i=0;
    while(Serial.available()>0){
      char tmpC = Serial.read();
      // serInByte[0] == motor1Speed
      // serInByte[1] == motor1Dir
      // serInByte[2] == motor2Speed
      // serinByte[3] == motor2Dir
      serInByte[i] = tmpC - '0'; 
      Serial.print(serInByte[i]);
      i++;     
    }    
    Serial.println();
    if (i == 4){
      Wire.beginTransmission(sdaMotorIndex);
      for(int i=0; i<4; i++){
        Wire.write(serInByte[i]);
      }
      Wire.endTransmission();
      Wire.beginTransmission(sdaLEDIndex);
      if (serInByte[0] == 0){
        Wire.write(0)
      }
      else{
        Wire.write(1);
      }
      Wire.endTransmission();
    }
    else{
      Serial.println("E");
    }
  }
  delay(1);
}

