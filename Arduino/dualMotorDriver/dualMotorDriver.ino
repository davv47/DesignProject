/*
  Output Signals to driver board
  
  Pins for output:
    Motor 1:
      12 & 11 control motor rotation
      10 PWM
      
    Motor 2:
      8 & 7 control motor rotation
      6 PWM
*/

/*dualDriverMotor************************************************
Code to recieve I2C and move motor accordingly
*****************************************************************/

#include <Wire.h>

int sdaIndex = 9;

const int inA1 = 4;
const int inB1 = 3;
const int M1PWM = 10;
const int cs1 = A3;

const int inA2 = 5;
const int inB2 = 2;
const int M2PWM = 11;
const int cs2 = A2;

const int speedMax = 255;

int highVal = 1023;
int lowVal = 0;
int M1Speed, M2Speed, speedAdjust;
int inA1Val, inB1Val, inA2Val, inB2Val;

int serInByte[4];

//int currentPin = A0;
//int currentVal;

void setup(){
  Serial.begin(9600);
  
  //Write Motor Speeds to motors  
  pinMode(inA1, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB2, OUTPUT);
  pinMode(M1PWM, OUTPUT);
  pinMode(M2PWM, OUTPUT); 
 
  Wire.begin(sdaIndex);   
  Wire.onReceive(receiveEvent); 
  
}

void loop(){
  delay(1);
}

void receiveEvent(int howMany){
  int i=0;
  while(Wire.available()){
    int c = Wire.read();
    Serial.println(c);
    serInByte[i] = c;
    i++;
  }
  moveMotors();
}

void moveMotors(){

    M1Speed = serInByte[0]*255/10*.8;
    M2Speed = serInByte[2]*255/10*.7;
    
    if(serInByte[1] == 0){
      inA1Val = lowVal;
      inB1Val = highVal;
    }
    else{
      inA1Val = highVal;
      inB1Val = lowVal;
    }
    if(serInByte[3] == 0){
      inA2Val = lowVal;
      inB2Val = highVal;
    }
    else{
      inA2Val = highVal;
      inB2Val = lowVal;
    }
    
  //Write Motor Speeds to motors
  analogWrite(inA1, inA1Val);
  analogWrite(inB1, inB1Val);  
  analogWrite(inA2, inA2Val);
  analogWrite(inB2, inB2Val);
  // output pwm to move wheel
  analogWrite(M1PWM, M1Speed);
  analogWrite(M2PWM, M2Speed); 
  Serial.println(M1Speed);
  
  // Read current
  /*
  if(0){
    currentVal = analogRead(currentPin);
    Serial.print("M1 speed =");
    Serial.print(M1Speed);
    Serial.print("\t M2 speed =");
    Serial.print(M2Speed);
    Serial.print("\t Current = ");
    Serial.println(currentVal);
  }
  */
}


  
