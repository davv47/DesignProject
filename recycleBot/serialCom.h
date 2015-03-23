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
        serialCom();
        ~serialCom();

        SerialStream ardu;

        // Global Declorations of Serial Information
        void open();
        void getStatus();
};

#endif // SERIALCOM_H
