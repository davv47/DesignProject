#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <iostream>
#include <string.h>
#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //Arduinio Device

using namespace std;
using namespace LibSerial;

const int BUFFER_SIZE = 4;
char inBuffer[BUFFER_SIZE];
SerialStream ardu;

class serialCom{
    public:
        // Global Declorations of Serial Information
        void open();
};

void serialCom::open(){
    /*The arduino must be setup to use the same baud rate*/
    ardu.Open(PORT);
    ardu.SetBaudRate(SerialStreamBuf::BAUD_9600);
    ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
}


#endif // SERIALCOM_H
