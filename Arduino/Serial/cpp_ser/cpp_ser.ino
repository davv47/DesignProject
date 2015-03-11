#include <Wire.h>

int ledPin = 13;
const int sdaMotorIndex = 9;
const int sdaLEDIndex = 8;
const int sdaActIndex = 10;
const int SDA_Pin = 2;
const int SCL_Pin = 3;

int serInByte[4];

void setup() {
  Serial.begin(9600);
  Wire.begin(sdaMotorIndex);
  //Wire.begin(sdaLEDIndex);
  //pinMode(SDA_Pin, INPUT);
  //pinMode(SCL_Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  
}
/*********************************************************
  Motor Signaling:
    serInByte[0] == motor1Speed
    serInByte[1] == motor1Dir
    serInByte[2] == motor2Speed
    serinByte[3] == motor2Dir
  Actuator Signaling
    serInByte[0] == Actuator Signal
    serInByet[1] == Actuator Dir (1 for Close, 0 for Open)
**********************************************************/
void loop(){
  if (Serial.available()>0){
    int i=0;
    while(Serial.available()>0){
      char tmpC = Serial.read();
      serInByte[i] = tmpC;
      i++;     
    }
    // Motor Signal
    if (i == 4 && serInByte[0] != 'A'){
      for (int i=0; i<4; i++){
        serInByte[i] = serInByte[i] - '0';
        //Serial.print(serInByte[i]);
      }
      //Serial.println();
      Wire.beginTransmission(sdaMotorIndex);
      for(int i=0; i<4; i++){
        Wire.write(serInByte[i]);
      }
      Wire.endTransmission();
      /*Wire.beginTransmission(sdaLEDIndex);
      int ledVal;
      if (serInByte[0] == 0){
       ledVal = 0;
      }
      else{
        ledVal = 1;
      }
      Wire.write(ledVal);
      Wire.endTransmission();*/
    }
    //Actuator Signal
    else if (serInByte[0] == 'A'){
      Wire.beginTransmission(sdaActIndex);
      //Send Direction Signal
      Wire.write(serInByte[1]);
      Wire.endTransmission();
    }
    else{
      Serial.println("E");
    }
  }
  delay(10);
}

