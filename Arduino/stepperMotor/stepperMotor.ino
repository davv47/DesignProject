/*stepperMotor***************************
Code to receieve data from I2C and move 
stepper motor number of steps
*****************************************/

#include <Wire.h>

int sdaIndex = 7;
int SDA_Pin = 2;
int SCL_Pin = 3;


int windA = A1;//black wire
int windB = A2;//red wire
int windC = A3;//green wire
int windD = A4;//blue wire

void setup()
{
  Wire.begin(sdaIndex);           // join i2c bus with address
  
  //Disable Internal Pullup Resistors
  pinMode(SDA_Pin, INPUT);
  pinMode(SCL_Pin, INPUT);
  
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(13, OUTPUT);
  
  pinMode(windA,OUTPUT);
  pinMode(windB,OUTPUT);
  pinMode(windC,OUTPUT);
  pinMode(windD,OUTPUT);
}

void loop()
{
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
  digitalWrite(13, HIGH);
  delay(1000);
  while(Wire.available()){
    int stp = Wire.read(); // receive byte as a character
    stepMotor(stp);
  }  
  digitalWrite(13, LOW);
  delay(1000);
}



void stepMotor(int numStep){
  for (int j=0; j<numStep; j++){
    for(unsigned char i = 0;i < 8; i++){
       stepSeq(i);
       delay(5);
    }
  }  
}

void stepSeq(unsigned char stepNumber){
  
  if(stepNumber == 0x00){
    digitalWrite(windA,HIGH);
    digitalWrite(windB,LOW);
    digitalWrite(windC,LOW);
    digitalWrite(windD,HIGH);
  }
  if(stepNumber == 0x01){
    digitalWrite(windA,HIGH);
    digitalWrite(windB,LOW);
    digitalWrite(windC,LOW);
    digitalWrite(windD,LOW);
  }
  if(stepNumber == 0x02){
    digitalWrite(windA,HIGH);
    digitalWrite(windB,HIGH);
    digitalWrite(windC,LOW);
    digitalWrite(windD,LOW);
  }
  if(stepNumber == 0x03){
    digitalWrite(windA,LOW);
    digitalWrite(windB,HIGH);
    digitalWrite(windC,LOW);
    digitalWrite(windD,LOW);
  }
  if(stepNumber == 0x04){
    digitalWrite(windA,LOW);
    digitalWrite(windB,HIGH);
    digitalWrite(windC,HIGH);
    digitalWrite(windD,LOW);
  }
  if(stepNumber == 0x05){
    digitalWrite(windA,LOW);
    digitalWrite(windB,LOW);
    digitalWrite(windC,HIGH);
    digitalWrite(windD,LOW);
  }
  if(stepNumber == 0x06){
    digitalWrite(windA,LOW);
    digitalWrite(windB,LOW);
    digitalWrite(windC,HIGH);
    digitalWrite(windD,HIGH);
  }
  if(stepNumber == 0x07){
    digitalWrite(windA,LOW);
    digitalWrite(windB,LOW);
    digitalWrite(windC,LOW);
    digitalWrite(windD,HIGH);
  }
}
