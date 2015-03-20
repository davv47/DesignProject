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
  if (Serial.available()>0){  
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
      int goInd;
      //Serial.println("In sensor");
      Wire.beginTransmission(sdaSensorIndex);
      int avail = Wire.requestFrom(sdaSensorIndex, 2);
      if(avail == 2){
        goInd = Wire.read();
        //Serial.println(goInd);
      }
      else{
        //Serial.print("Unexpected number of bytes received: ");
        //Serial.println(avail);
      }
      Wire.endTransmission();
      digitalWrite(ledPin, LOW);
      int tmpWrite;
      //Stop moving Forward
      if (goInd == 1){      
        tmpWrite = 10;
      }
      //Move forward
      else{
        tmpWrite = 55;
      }
      Serial.println(tmpWrite); // write to serialstream
    }        
    
    //Actuator Signal
    else if (serInByte[0] == 4){
      int finInd;
      int dir = serInByte[1];      
      Wire.beginTransmission(sdaActIndex);
      //Serial.println("Started");
      dir = 1;
      Wire.write(dir);
      //Serial.println("Written");
      Wire.endTransmission();
    }
    else{
      //Serial.println("E");
    }
  }
  delay(1);
}

