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

int sdaIndex = 8;

const int inA1 = 4;
const int inB1 = 5;
const int M1PWM = A0;
const int cs1 = 6;

const int inA2 = 8;
const int inB2 = 7;
const int M2PWM = A1;
const int cs2 = 9;

const int speedMax = 255;

int highVal = 1023;
int lowVal = 0;
int M1Speed, M2Speed, speedAdjust;
int inA1Val, inB1Val, inA2Val, inB2Val;

int currentPin = A0;
int currentVal;

void setup(){
  Serial.begin(9600);
  speedAdjust = 1;
  // Set Inital Direction
  inA1Val = highVal;
  inB1Val = lowVal;
  inA2Val = lowVal;
  inB2Val = highVal;
  //Write Motor Speeds to motors
  analogWrite(inA1, inA1Val);
  analogWrite(inB1, inB1Val);  
  analogWrite(inA2, inA2Val);
  analogWrite(inB2, inB2Val);
}

void loop(){
  if (M1Speed >= speedMax){
    speedAdjust = -1;
  }
  
  if (M1Speed <= speedMax*.05){
    speedAdjust = 1;
    // Change Direction
    if (inA1Val == highVal){
      inA1Val = lowVal;
      inB1Val = highVal;
      inA2Val = highVal;
      inB2Val = lowVal;
    }
    else{
      inA1Val = highVal;
      inB1Val = lowVal;
      inA2Val = lowVal;
      inB2Val = highVal;
    }
  }  
  
  // change speed
  M1Speed += speedAdjust;
  M2Speed += speedAdjust;
  //Write Motor Speeds to motors
  analogWrite(inA1, inA1Val);
  analogWrite(inB1, inB1Val);  
  analogWrite(inA2, inA2Val);
  analogWrite(inB2, inB2Val);
  // output pwm to move wheel
  analogWrite(M1PWM, M1Speed);
  analogWrite(M2PWM, M2Speed); 
  
  
  // Read current
  currentVal = analogRead(currentPin);
  Serial.print("M1 speed =");
  Serial.print(M1Speed);
  Serial.print("\t M2 speed =");
  Serial.print(M2Speed);
  Serial.print("\t Current = ");
  Serial.println(currentVal);
  
  delay(10);
  
}



  
