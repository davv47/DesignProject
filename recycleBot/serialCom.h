#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <iostream>
#include <string.h>
#include <SerialStream.h>
#define PORT "/dev/ttyACM2" //Arduinio Device

using namespace std;
using namespace LibSerial;

const int BUFFER_SIZE = 4;
char inBuffer[BUFFER_SIZE];
SerialStream ardu;

class serialCom{
    public:
        // Global Declorations of Serial Information
        void open();
        void getStatus();
};

/*serialCom********************************************************************
 *Initalizes the PORT and open it for serial communication
 * ***************************************************************************/
void serialCom::open(){
    /*The arduino must be setup to use the same baud rate*/
    ardu.Open(PORT);
    ardu.SetBaudRate(SerialStreamBuf::BAUD_9600);
    ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
}

/*getStatus********************************************************************
 *WORK IN PROGRESS
 * Get status of uC being sent through Serial
 * ***************************************************************************/
void getStatus(){
    char status[4];
    for (int i = 0; i < 4; i++){
        ardu >> status[i];
    }
    string str = string(status);
}


#endif // SERIALCOM_H
