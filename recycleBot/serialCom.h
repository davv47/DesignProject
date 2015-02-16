#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <iostream>
#include <string.h>
#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //Arduinio Device

using namespace std;
using namespace LibSerial;

class serialCom{
    public:
        // Global Declorations of Serial Information
        SerialStream ardu;
        const int BUFFER_SIZE = 1;
        char inBuffer[BUFFER_SIZE];

};



#endif // SERIALCOM_H
