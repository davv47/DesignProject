#include <Wire.h>

int ledPin = 13;
const int sdaMotorIndex = 9;
const int sdaActIndex = 10;
const int sdaSensorIndex = 11;
const int SDA_Pin = 2;
const int SCL_Pin = 3;

int serInByte[4];

void setup() {
  Serial.begin(9600);
  Wire.begin(sdaMotorIndex);
  Wire.begin(sdaSensorIndex);
  Wire.begin(sdaActIndex);
  //pinMode(SDA_Pin, INPUT);
  //pinMode(SCL_Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  serInByte[0] = 4;
  
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
  Senor Signaling
    serInByte[0] == Sensor Signal
**********************************************************/
void loop(){
  //if (Serial.available()>0){  
    int i=0;
    while(Serial.available()>0){
      char tmpC = Serial.read();
      serInByte[i] = tmpC - '0';
      //Serial.print(tmpC);
      i++;     
    }
    // Motor Signal
    if (serInByte[0] ==  1){
      for (int i=0; i<4; i++){
        serInByte[i] = serInByte[i];// - '0';
        //Serial.print(serInByte[i]);
      }
      //Serial.println();
      Wire.beginTransmission(sdaMotorIndex);
      for(int i=0; i<4; i++){
        Wire.write(serInByte[i]);
      }
      Wire.endTransmission();
      
    }
    //Sensor Signal
    else if (serInByte[0] == 3){
      digitalWrite(ledPin, HIGH);
      byte goInd;
      //Serial.println("In sensor");
      Wire.beginTransmission(sdaSensorIndex);
      Wire.requestFrom(sdaSensorIndex, 1);
      while(Wire.available()){
        goInd = Wire.read();
      }
      Wire.endTransmission();
      digitalWrite(ledPin, LOW);      
      Serial.println(goInd); // write
    }      
    //Actuator Signal
    else if (serInByte[0] == 4){
      int finInd;
      Wire.beginTransmission(sdaActIndex);
      //Send Direction Signal
      //Wire.write(serInByte[1]);
      Wire.write('C');
      Wire.endTransmission();
      Serial.println("Writing Direction");
      Wire.requestFrom(sdaActIndex, 1);
      while(Wire.available()){
        finInd = Wire.read();
      }
      Serial.println("Move Finished");
      Serial.write(finInd);
    }
    else{
      //Serial.println("E");
    }
  //}
  delay(1);
}

