#include "serialCom.h"

#include <iostream>
#include <string.h>
#include <SerialStream.h>

serialCom::serialCom(){}
serialCom::~serialCom(){}

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
void serialCom::getStatus(){
    char status[4];
    for (int i = 0; i < 4; i++){
        ardu >> status[i];
    }
    string str = string(status);
}


