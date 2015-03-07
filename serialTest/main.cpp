#include <iostream>

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //Arduinio Device

using namespace std;
using namespace LibSerial;

SerialStream ardu;
const int BUFFER_SIZE = 1;
char inBuffer[BUFFER_SIZE];

void open();
void stepperMotor();

int main(){
    open();
    char status[4];
    for (int i = 0; i < 4; i++){
        ardu >> status[i];
    }
    string str = string(status);
    cout<<str;
}

void stepperMotor(){
    int stepNum;
    cout<<"Input number of Steps:"<<endl;
    cin>>stepNum;
    ardu.write(inBuffer, BUFFER_SIZE);
}

void motors(){
    int m1Speed, m2Speed;
    cout<<"Input Speed of Motor1 and Motor2";
}

void open(){
    /*The arduino must be setup to use the same baud rate*/
    ardu.Open(PORT);
    ardu.SetBaudRate(SerialStreamBuf::BAUD_9600);
    ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
}
