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
  
  /*Disable Internal Pullup Resistors
  pinMode(SDA_Pin, INPUT);
  pinMode(SCL_Pin, INPUT);  */
  
  Wire.onRequest(requestEvent);
  pinMode(ledPin, OUTPUT);
  
  pinMode(sensePin, INPUT);
}

void loop(){
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent(){
  byte dist = 1;//getSenseDist();
  digitalWrite(ledPin, HIGH);
  Wire.write(dist);
  digitalWrite(ledPin, LOW);
}

/**getSenseDist*******************************************************
Get sensor distance from object
**********************************************************/
byte getSenseDist(){
  int senseVals = 10;
  int val = 0;
  byte ind;
  int valThresh = 450;
  for (int i=0; i<senseVals; i++){
    val = val + analogRead(A2);
    delay(1);
  }
  val = val/senseVals;
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

