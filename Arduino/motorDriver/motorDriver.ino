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

const int M1R1 = 4;
const int M1R2 = 2;
const int M1PWM = 3;

const int M2R1 = 8;
const int M2R2 = 7;
const int M2PWM = 6;
const int speedMax = 255;

int highVal = 1023;
int lowVal = 0;
int M1Speed, M2Speed, speedAdjust;
int M1R1Val, M1R2Val, M2R1Val, M2R2Val;

int currentPin = A0;
int currentVal;

void setup(){
  Serial.begin(9600);
  speedAdjust = 1;
  // Set Inital Direction
  M1R1Val = highVal;
  M1R2Val = lowVal;
  M2R1Val = lowVal;
  M2R2Val = highVal;  
  //Write Motor Speeds to motors
  analogWrite(M1R1, M1R1Val);
  analogWrite(M1R2, M1R2Val);  
  analogWrite(M2R1, M2R1Val);
  analogWrite(M2R2, M2R2Val);
}

void loop(){
  if (M1Speed >= speedMax){
    speedAdjust = -1;
  }

  if (M1Speed <= speedMax*.05){
    speedAdjust = 1;
    // Change Direction
    if (M1R1Val == highVal){
      M1R1Val = lowVal;
      M1R2Val = highVal;
      M2R1Val = highVal;
      M2R2Val = lowVal;
    }
    else{
      M1R1Val = highVal;
      M1R2Val = lowVal;
      M2R1Val = lowVal;
      M2R2Val = highVal;
    }
  }  

  // change speed
  M1Speed += speedAdjust;
  M2Speed += speedAdjust;
  //Write Motor Speeds to motors
  analogWrite(M1R1, M1R1Val);
  analogWrite(M1R2, M1R2Val);  
  analogWrite(M2R1, M2R1Val);
  analogWrite(M2R2, M2R2Val);
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





