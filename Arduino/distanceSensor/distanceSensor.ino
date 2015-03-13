/*distanceSensorr***************************
code to get sensor readings
*****************************************/

#include <Wire.h>

int sdaIndex =11;
int SDA_Pin = 2;
int SCL_Pin = 3;

//Pin analog output of senesor is attached to
int sensePin = A2;

void setup(){
  Wire.begin(sdaIndex);           // join i2c bus with address
  
  /*Disable Internal Pullup Resistors
  pinMode(SDA_Pin, INPUT);
  pinMode(SCL_Pin, INPUT);
  */
  
  Wire.onRequest(requestEvent);
  Serial.begin(9600);           // start serial for output
  pinMode(13, OUTPUT);
  
  pinMode(sensePin, INPUT);
}

void loop(){
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent(){
  int dist = getSenseDist();
  Wire.write(dist);
}

/**getSenseDist*******************************************************
Get sensor distance from object
**********************************************************/
int getSenseDist(){
  int senseVals = 10;
  int val = 0;
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
  return val;
}

