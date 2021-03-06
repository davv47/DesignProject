#include "nav.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include "morphology.h"
#include "objRecognition.h"
#include "geometry.h"
#include "imgProcess.h"

const int BUFFER_SIZE = 4;
char inBuffer[BUFFER_SIZE];

nav::nav(){
    imgFindObj = "findObj";
    imgMoveToObj = "moveToObj";
}

nav::~nav(){}

/*startMove********************************************************************
 *Function to start navigation class.
 * ***************************************************************************/
void nav::startNav(string colour){
    userStopped = false;
    checkForStop(1000);
    actDir = '1';
    stepperMotor();
    actDir ='0';
    stepperMotor();
    hasObj = false;
    while(!hasObj && !userStopped){
        moveToObj(colour);
        findObj(colour);
    }
    cout<<"At Object Rough Position"<<endl;
    closeMoveObj();
    stepperMotor();
    hasObj = true;
    cout<<"Got Object"<<endl;
    while(hasObj && !userStopped){
        moveToObj(colour);
        findObj(colour);
    }
    cout<<"At Pile Rough Position"<<endl;
    closeMovePile(colour);
    stepperMotor();
    cout<<"Object Sorted"<<endl;
    leaveArea();
}

/*moveToObj********************************************************************
 *Function for Robot to follow red object
 * ***************************************************************************/
void nav::moveToObj(string colour){
    if (userStopped){
        return;
    }
    waitForSlow(1000);
    VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);

    //Get centroid of object
    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    char dir, lastDir;
    int deadX = 25;
    int areaNoObj = 150;
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
            cout<<"Before Show"<<endl;
            imshow(imgMoveToObj, imgOrig);
            cout<<"After Show"<<endl;
            checkForStop(1);
        }
        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);
        //imshow("imgBW", imgOut);
        int x = objectPT.x;
        cout<<x<<endl;
        geometry geo;
        int area;
        if (x){
            area = geo.sizeLargest(imgOut, rect);
        }
        else{
            area = 0;
        }
        if (area < areaNoObj){
            cout<<"Too Small"<<endl;
            cvDestroyWindow(imgMoveToObj);
            waitForSlow(500);
            return;

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
                if (!hasObj){   //If there isnt a object in the actuator, close it
                    actDir = '1';
                    hasObj = true;
                    return;
                }
                else{
                    actDir = '0';
                    hasObj = false;
                    return;
                }
                loop = false;
            }
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
void nav::findObj(string colour){
    if (userStopped) return;
    waitForSlow(1000);
    VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);

    //Get centroid of object
    int loopCount = 0;
    int loopMax = 1;

    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int areaNoObj = 200;
    waitTime = 2;
    longWaitTime = 10;
    loop = true;

    namedWindow(imgFindObj, CV_WINDOW_AUTOSIZE);

    while(this->loop){
        Point objectPT;
        imgPro.capFrame(cap, imgOut, imgOrig, colour);
        if (!imgOrig.empty()){
            imshow(imgFindObj, imgOrig);
            checkForStop(1);
        }

        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);
        //imshow("imgBW", imgOut);
        int x = objectPT.x;
        cout<<x<<endl;
        geometry geo;
        int area;
        if (x){
            area = geo.sizeLargest(imgOut, rect);
        }
        else{
            area = 0;
        }
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
            waitForSlow(500);
            return;
        }
    }
    //cvDestroyWindow(imgFindObj);
}

/*closeMoveObj********************************************************************
 * fine approach using sensor
 * ***************************************************************************/
void nav::closeMoveObj(){
    if (userStopped){
        return;
    }
    waitForSlow(1000);
    char str[2];
    loop = true;
    waitTime = 2;
    longWaitTime = 100;
    char m1Speed = '1';
    char m1Dir = '1';
    char m2Speed = '1';
    char m2Dir = '1';
    int count10;

    while(loop){

        inBuffer[0] = '3';
        inBuffer[1] = 'X';
        inBuffer[2] = 'X';
        inBuffer[3] = 'X';
        //Open Serial Port
        ser.open();
        //Send data
        ser.ardu.write(inBuffer, BUFFER_SIZE);
        //Read Response
        ser.ardu>>str;
        //close Serial Port
        ser.ardu.Close();
        cout<<str<<endl;

        //Move forward a bit more
        if (strcmp("55", str) == 0){
            sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
            waitForSlow(longWaitTime);
        }
        //End of step forward
        else if(strcmp("10", str) == 0){
            count10++;
            if(count10 >= 10){
                cout<<"Got to object"<<endl;
                waitForSlow(longWaitTime);
                return;
            }
            else{
                cout<<"Counting 10s"<<endl;
                sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
                waitForSlow(longWaitTime);
            }
        }
        //Error
        else{
            cout<<"in error :("<<endl;
            waitForSlow(longWaitTime);
        }

    }
}

/*closeMovePile********************************************************************
 * fine approach using sensor
 * ***************************************************************************/
void nav::closeMovePile(string colour){
    if (userStopped){
        return;
    }
    waitForSlow(500);
    VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);


    //Get centroid of object
    Rect rect;
    char m1Speed = '1';
    char m1Dir = '1';
    char m2Speed = '1';
    char m2Dir = '1';

    int areaDropOff = 2500;
    waitTime = 2;
    longWaitTime = 100;
    this->loop = true;

    namedWindow(imgMoveToObj, CV_WINDOW_AUTOSIZE);

    imgPro.capFrame(cap, imgOut, imgOrig, colour);
    imshow(imgMoveToObj, imgOrig);

    //While program not stopped by user
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

        geometry geo;
        int area = geo.sizeLargest(imgOut, rect);
        if (area > areaDropOff){

            cout<<"Got to Pile"<<endl;
            waitForSlow(longWaitTime);
            return;
        }
        else{
            sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
            waitForSlow(longWaitTime);
        }
    }
}

/*stepperMotor********************************************************************
 * Close Or Open stepper motor
 * ***************************************************************************/
void nav::stepperMotor(){
    if (userStopped){
        return;
    }
    waitForSlow(500);
    waitTime = 5;
    inBuffer[0] = '4';
    inBuffer[1] = actDir;
    inBuffer[2] = 'X';
    inBuffer[3] = 'X';
    //Open Serial Port
    ser.open();
    //Send data
    ser.ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ser.ardu.Close();
    cout<<"Moved Stepper Motors Direction: "<<actDir<<endl;
    waitForSlow(1000);
}

/*leaveArea********************************************************************
 * Leave Area after object is dropped off
 * ***************************************************************************/
void nav::leaveArea(){
    if (userStopped){
        return;
    }
    char m1Speed, m1Dir, m2Speed, m2Dir;
    waitTime = 2;
    longWaitTime = 10;

    //Back Off
    m1Speed = '1';
    m1Dir = '0';
    m2Speed = '1';
    m2Dir = '0';
    for (int i = 0; i<20; i++){
        sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
        waitForSlow(longWaitTime);
    }

    //Turn Slightly
    m1Speed = '1';
    m2Speed = '0';
    m1Dir = '1';
    m2Dir = '1';
    for (int i = 0; i<10; i++){
        sendMove(m1Speed, m2Speed, m1Dir, m2Dir);
        waitForSlow(longWaitTime);
    }
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
    ser.ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ser.ardu.Close();
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
    int sizeThresh = 3000;
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
    ser.ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ser.ardu.Close();
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
    ser.ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ser.ardu.Close();
    cout<<"Program stopped by user"<<endl;
    this->loop = false;
    userStopped = true;
}
