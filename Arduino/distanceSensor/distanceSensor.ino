/*distanceSensorr***************************
code to get sensor readings
*****************************************/

#include <Wire.h>

int sdaIndex = 11;
int SDA_Pin = 2;
int SCL_Pin = 3;

//Pin analog output of senesor is attached to
int sensePin = A2;
int ledPin = 13;

void setup(){
  Wire.begin(sdaIndex);           // join i2c bus with address
  
  //Disable Internal Pullup Resistors
  pinMode(SDA_Pin, INPUT);
  pinMode(SCL_Pin, INPUT);
  
  Wire.onRequest(requestEvent);
  pinMode(ledPin, OUTPUT);  
  pinMode(sensePin, INPUT);
  
  digitalWrite(ledPin, LOW);
  
}

void loop(){
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent(){
  int dist = getSenseDist();
  if (dist == 1){
    Wire.write(1); //Done Moving Towards object
  }
  else{
    Wire.write(0); //Move Towards Object
  }
}

/**getSenseDist*******************************************************
Get sensor distance from object
**********************************************************/
int getSenseDist(){
  int senseVals = 50;
  int val = 0;
  byte ind;
  int valThresh = 400;
  for (int i=0; i<senseVals; i++){
    val = val + analogRead(A2);
    delay(1);
  }
  val = val/senseVals;
  //Close to object
  if (val>valThresh){
    val = 1;
  }
  else{
    val = 0;
  }
  if (val == 1){
    ind = 1;
  }
  else{
    ind = 0;
  }
  return ind;
}

