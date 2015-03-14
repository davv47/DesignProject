/*stepperMotor***************************
Code to receieve data from I2C and move 
stepper motor number of steps
*****************************************/

#include <Wire.h>
#include <Stepper.h>

int sdaIndex =10;
int SDA_Pin = 2;
int SCL_Pin = 3;

int windPA = 5;//black wire
int windPB = 6;//red wire
int windPC = 2;//green wire
int windPD = 3;//blue wire

int windSA = A0;//black wire
int windSB = A1;//red wire
int windSC = A2;//green wire
int windSD = A3;//blue wire

int tp = 7;//Changed to Analog Pin
int ts = 8; //Changed to Analog Pin

char dir;

int stepperMotorSteps = 200;

//200 step stepper motor
Stepper stpPort = Stepper(stepperMotorSteps, windPA, windPC, windPB, windPD);
Stepper stpStar = Stepper(stepperMotorSteps, windSA, windSC, windSB, windSD);

void setup(){
  Wire.begin(sdaIndex);           // join i2c bus with address
  
  //Disable Internal Pullup Resistors
  //pinMode(SDA_Pin, INPUT);
  //pinMode(SCL_Pin, INPUT);
  
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(13, OUTPUT);
  
  pinMode(windPA,OUTPUT);
  pinMode(windPB,OUTPUT);
  pinMode(windPC,OUTPUT);
  pinMode(windPD,OUTPUT);
  
  pinMode(windSA,OUTPUT);
  pinMode(windSB,OUTPUT);
  pinMode(windSC,OUTPUT);
  pinMode(windSD,OUTPUT);
  
  pinMode(tp, INPUT);
  pinMode(ts, INPUT);
  
  stpPort.setSpeed(10); //set motor speed to 30 rpm
  stpPort.setSpeed(10); //set motor speed to 30 rpm
}

void loop(){
  int steps = 20;
  delay(1000);
  dir = 'C';
  //moveAct();
  stpPort.step(steps);
  stpStar.step(-1*steps*);
  delay(1000);
  dir = 'O';
  stpPort.step(-1*steps);
  stpStar.step(steps);
  //moveAct();
}

void receiveEvent(int howMany){
  while(Wire.available()){
    dir = Wire.read();
  }
}

void requestEvent(){
  moveAct();
  Wire.write(1);
}


/**moveAct*******************************************************
Code to close actuator flaps
Runs 'till tactile sensors are triggered
**********************************************************/
void moveAct(){
  int maxStep = stepperMotorSteps/4*.8; //80% of 90 degree rurn 
  int i = 0;
  int dirInd;
  int numSteps = 10;
  if (dir =='C'){
    dirInd = 1;
  }
  else{
    dirInd = -1;
  }    
  boolean boolTouchPort = checkPin(tp);
  boolean boolTouchStar = checkPin(ts);
  while(i<maxStep && (!boolTouchPort || !boolTouchStar)){
    boolTouchPort = checkPin(tp);
    boolTouchStar = checkPin(ts);
    stpPort.step(numSteps*dirInd);
    stpStar.step(-1*numSteps*dirInd);
    i = i + numSteps;
  }
}

/**checkPin*******************************************************
Code to open actuator flaps
How you say? I'm not sure yet
**********************************************************/
boolean checkPin(int pin){
  boolean boolTouchPort = false;
  int highThresh = 1024/2;
  int tmpVal = analogRead(pin);
  if (tmpVal <= highThresh){
    boolTouchPort = true;
  }
  else{
    boolTouchPort = true;
  }
  return boolTouchPort;
}
