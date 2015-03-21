#ifndef NAV_H
#define NAV_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include <serialCom.h>
#include <morphology.h>
#include <objRecognition.h>
#include <geometry.h>
#include <objectfollow.h>
#include "imgProcess.h"

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //ser.arduinio Device

using namespace std;
using namespace cv;

class nav{
    public:
    serialCom ser;
    imgProcess imgPro;
    int waitTime;
    int longWaitTime;
    bool loop;
    bool hasObj;
    Mat imgOrig, imgOut;
    const char* imgFindObj = "findObj";
    const char* imgMoveToObj = "moveToObj";

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

/*startMove********************************************************************
 *Function to start navigation class.
 * ***************************************************************************/
void nav::startNav(string colour){
    VideoCapture cap(imgPro.webCamNum);
    //imgPro.openWebcam(cap);
    checkForStop(1000);
    moveToObj(colour, cap);
}

/*moveToObj********************************************************************
 *Function for Robot to follow red object
 * ***************************************************************************/
void nav::moveToObj(string colour, VideoCapture cap){
    //VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);

    //Get centroid of object
    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    char dir, lastDir;
    int deadX = 50;
    int areaNoObj = 300;
    waitTime = 2;
    longWaitTime = 10;
    this->loop = true;

    namedWindow(imgMoveToObj, CV_WINDOW_AUTOSIZE);

    //While program not stopped by user
    int loopCount = 0;
    int loopMax = 3;
    while (this->loop){
        Point objectPT;
        imgPro.capFrame(cap, imgOut, imgOrig, colour);
        if (!imgOrig.empty()){
            imshow(imgMoveToObj, imgOrig);
            checkForStop(1);
        }
        //imshow("imgBW", imgOut);
        //Point objectPT = objGeo.centre(imgBW);
        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);

        int x = objectPT.x;
        geometry geo;
        int area = geo.sizeLargest(imgOut, rect);
        if (area < areaNoObj){
            cout<<"Too Small"<<endl;
            cvDestroyWindow(imgMoveToObj);
            //sendMove('0', '0', '0', '0');
            //checkForStop(1000);
            waitForSlow(500);
            findObj(colour, cap);
            loop = false;
        }
        else{
            //If no object in frame treat object as in centre of window
            if (x<0){
                x = imgPro.xRez/2-1;
            }
            x = x-imgPro.xRez/2-1;

            //Move Motors
            //If approximately in middle then move in straight line
            if (abs(x) < deadX){
               moveLine(area, m1Speed, m2Speed, m1Dir, m2Dir, dir);
                x = 0;
            }
            // If in left of frame move motors Dir 1
            if(x>0){
                dir = 'R';
                m1Speed = '1';
                m2Speed = '0';
                m1Dir = '0';
                m2Dir = '0';
            }
            else if (x<0){
                dir = 'L';
                m1Speed = '1';
                m2Speed = '0';
                m1Dir = '1';
                m2Dir = '1';
            }

            //Send movement Signal to motors
            sendMove(m1Speed, m2Speed, m1Dir, m2Dir);

            //If direction has changed the stop movement temporarly
            if (dir != lastDir){
                waitForSlow(250);
            }
            bool stepDir = false;
            if (dir =='R' || dir == 'L' || dir == 'B' || dir == 'N'){
                stepDir = true;
            }
            else if (dir == 'S'){
                cout<<"Stopped->Finding Object"<<endl;
                cvDestroyWindow(imgMoveToObj);
                waitForSlow(longWaitTime);
                closeMove(colour);
                loop = false;
            }
            /*
            if (stepDir){
                loopCount = loopMax;
            }*/
            if (loop && (stepDir || loopCount == loopMax)){
                loopCount = 0;
                waitForSlow(waitTime);
            }
            // Output x
            cout<<"dir is: "<<dir<<" x is: "<<x<<" Area is: "<<area<<" stepDir is: "<< stepDir<<endl;
            //cout<<inBuffer<<endl;
            loopCount++;
            lastDir = dir;
        }
    }
    //cvDestroyWindow(imgMoveToObj);
}

/*findObj********************************************************************
 * Find Object in working area
 * ***************************************************************************/
void nav::findObj(string colour, VideoCapture cap){
    //VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);

    //Get centroid of object
    int loopCount = 0;
    int loopMax = 1;

    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int forwardCnt = 10;
    int turnCnt = 3;
    int areaNoObj = 100;
    waitTime = 2;
    longWaitTime = 10;
    this->loop = true;
    namedWindow(imgFindObj, CV_WINDOW_AUTOSIZE);
    while(this->loop){
        Point objectPT;
        imgPro.capFrame(cap, imgOut, imgOrig, colour);        
        if (!imgOrig.empty()){
            imshow(imgFindObj, imgOrig);
            checkForStop(1);
        }

        //imshow("imgBW", imgOut);
        //Point objectPT = objGeo.centre(imgBW);
        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);

        int x = objectPT.x;
        geometry geo;
        int area = geo.sizeLargest(imgOut, rect);
        //Area is too small--contine looking
        if (area < areaNoObj){
            cout<<"Too Small"<<endl;

            //Turn Right Slightly
            m1Speed = '1';
            m2Speed = '0';
            m1Dir = '1';
            m2Dir = '1';
            //i = 0;
            sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
            if (loop && loopCount >= loopMax){
                waitForSlow(waitTime);
                loopCount = 0;
            }
            loopCount++;
            // Output x
            cout<<"x is: "<<x<<" Area is: "<<area<<endl;
            cout<<inBuffer<<endl;
        }
        //Area is big enough--proceed to object
        else{
            cvDestroyWindow(imgFindObj);
            //sendMove('0', '0', '0', '0');
            //checkForStop(1000);
            waitForSlow(500);
            moveToObj(colour, cap);            
            loop = false;
        }
    }
    //cvDestroyWindow(imgFindObj);
}

/*closeMove********************************************************************
 * fine approach using sensor
 * ***************************************************************************/
void nav::closeMove(string colour){
    char str[2];
    loop = true;
    waitTime = 2;
    longWaitTime = 100;
    char m1Speed = '1';
    char m1Dir = '1';
    char m2Speed = '1';
    char m2Dir = '1';

    while(loop){

        inBuffer[0] = '3';
        inBuffer[1] = 'X';
        inBuffer[2] = 'X';
        inBuffer[3] = 'X';
        //Open Serial Port
        ser.open();
        //Send data
        ardu.write(inBuffer, BUFFER_SIZE);
        //Read Response
        ardu>>str;
        //close Serial Port
        ardu.Close();
        cout<<str<<endl;

        //Move forward a bit more
        if (strcmp("55", str) == 0){
            sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
            waitForSlow(longWaitTime);
        }
        //End of step forward
        else if(strcmp("10", str) == 0){
            cout<<"Got to object"<<endl;
            waitForSlow(longWaitTime);
            stepperMotor('1', colour);
            loop = false;
        }
        //Error
        else{
            cout<<"in error :("<<endl;
            waitForSlow(longWaitTime);
        }

    }
}

/*stepperMotor********************************************************************
 * fine approach using sensor
 * ***************************************************************************/
void nav::stepperMotor(char dir, string colour){
    loop = true;
    waitTime = 5;
    inBuffer[0] = '4';
    inBuffer[1] = dir;
    inBuffer[2] = 'X';
    inBuffer[3] = 'X';
    //Open Serial Port
    ser.open();
    //Send data
    ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ardu.Close();
    startNav(colour);
    loop = false;
}

/*waitForSlow********************************************************************
 * Delay changing directions (do not blow motors)
 * ***************************************************************************/
void nav::waitForSlow(int t){
    char m1Speed, m2Speed, m1Dir, m2Dir;
    m1Speed = '0';
    m2Speed = '0';
    m1Dir = '0';
    m2Dir = '0';
    //Send Signal to motors
    inBuffer[0] = m1Speed;
    inBuffer[1] = m1Dir;
    inBuffer[2] = m2Speed;
    inBuffer[3] = m2Dir;
    //Open Serial Port
    ser.open();
    //Send data
    ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ardu.Close();
    cout<<"Wait For Slow: "<<inBuffer<<endl;
    checkForStop(t);    
}

/*moveLine********************************************************************
 * Move object forward or backword based on its size
 * Input:
 *      a - area of object in image
 * Modifies:
 *      -Motor Speeds
 *      -Motor Directions
 * ***************************************************************************/
void nav::moveLine(int a, char& m1Speed, char& m2Speed, char& m1Dir, char& m2Dir, char& dir){
    char sizeRng;
    int sizeThresh = 15000;
    int deadZone = 2000/2;
    int sizeSlow = 2000;
    if (a>sizeSlow && a<(sizeThresh-deadZone)) sizeRng = 'N';
    else if (a<sizeSlow) sizeRng = 'F';
    else if (a>=(sizeThresh+deadZone)) sizeRng = 'B';
    else sizeRng = 'S';
    switch (sizeRng){
    case 'F':
        dir = 'F'; //normally 'F' limiting to slow forward
        m1Speed = '1';
        m1Dir = '1';
        m2Speed = '1';        
        m2Dir = '1';
        cout<<"Fast foreward"<<endl;
        break;
    case 'N':
        dir = 'N';
        m1Speed = '1';
        m1Dir = '1';
        m2Speed = '1';
        m2Dir = '1';
        cout<<"Slow foreward"<<endl;
        break;
    case 'B':
        dir = 'B';
        m1Speed = '1';
        m1Dir = '0';
        m2Speed = '1';
        m2Dir = '0';
        cout<<"Going backward"<<endl;
        break;
    case 'S':
        dir = 'S';
        m1Speed = '0';
        m1Dir = '0';
        m2Speed = '0';
        m2Dir = '0';
        cout<<"Stopped"<<endl;
        break;
    default:
        dir = 'S';
        m1Speed = '0';
        m1Dir = '0';
        m2Speed = '0';
        m2Dir = '0';
        cout<<"Default"<<endl;
        break;
    }
}

/*checkForStop********************************************************************
 * Check if stop should be done
 * ***************************************************************************/
void nav::checkForStop(int time){
    char tmp = waitKey(time);
    if (tmp == 'c' || tmp == 'C'){
        this->loop = false;
        stopMovement();
    }
}

/*sendMove********************************************************************
 * Sends motor movement signals to serial port
 * ***************************************************************************/
void nav::sendMove(char m1Speed, char m2Speed, char m1Dir, char m2Dir){
    inBuffer[0] = m1Speed;
    inBuffer[1] = m1Dir;
    inBuffer[2] = m2Speed;
    inBuffer[3] = m2Dir;

    //Open Serial Port
    ser.open();
    //Send data
    ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ardu.Close();
    //Delay to account for small move & check for end program signal
    checkForStop(waitTime);
    cout<<"Send Move: "<<inBuffer<<" "<<waitTime<<endl;
}

/*stopMovement********************************************************************
 * Sets the motor speed of all motors to zero (for end of the program)
 * ***************************************************************************/
void nav::stopMovement(){
    char m1Speed, m2Speed, m1Dir, m2Dir;
    m1Speed = '0';
    m2Speed = '0';
    m1Dir = '0';
    m2Dir = '0';
    //Send Signal to motors
    inBuffer[0] = m1Speed;
    inBuffer[1] = m1Dir;
    inBuffer[2] = m2Speed;
    inBuffer[3] = m2Dir;
    //Open Serial Port
    ser.open();
    //Send data
    ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ardu.Close();
    cout<<"Program stopped by user"<<endl;
    this->loop = false;
}



#endif // NAV_H
