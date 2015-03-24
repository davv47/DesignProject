#ifndef NAV_H
#define NAV_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include "serialCom.h"
#include "imgProcess.h"

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //ser.arduinio Device

using namespace std;
using namespace cv;

class nav{
    public:

    nav();
    ~nav();

    serialCom ser;
    imgProcess imgPro;
    int waitTime, longWaitTime;
    bool loop, hasObj, userStopped;

    char actDir;
    Mat imgOrig, imgOut;
    const char* imgFindObj;
    const char* imgMoveToObj;

    void startNav(string);
    void moveToObj(string);
    void findObj(string);
    void closeMoveObj();
    void closeMovePile(string);
    void stepperMotor();
    void leaveArea();

    void waitForSlow(int);
    void moveLine(int, char&, char&, char&, char&, char&);
    void checkForStop(int);
    void sendMove(char, char, char, char);
    void stopMovement();

};

#endif // NAV_H
