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
    int waitTime;
    int longWaitTime;
    bool loop;
    bool hasObj;
    Mat imgOrig, imgOut;
    const char* imgFindObj;
    const char* imgMoveToObj;

    void startNav(string);
    void moveToObj(string, VideoCapture);
    void findObj(string, VideoCapture);
    void closeMove(string);

    void waitForSlow(int);
    void moveLine(int, char&, char&, char&, char&, char&);
    void checkForStop(int);
    void sendMove(char, char, char, char);
    void stopMovement();
    void stepperMotor(char, string);
};

#endif // NAV_H
