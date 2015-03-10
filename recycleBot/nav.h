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
    imgProcess imgPro;
    int waitTime;
    bool loop;
    Mat imgOrig, imgOut;
    const char* imgName = "Bounding Image";

    void startNav(string);
    void moveToObj(string, VideoCapture);
     void findObj(string, VideoCapture);

    void waitForSlow(int, serialCom);
    void moveLine(int, char&, char&, char&, char&, char&);
    void checkForStop(serialCom, int, bool&);
    void sendMove(serialCom, char, char, char, char);
    void stopMovement(serialCom);   
};

/*startMove********************************************************************
 *Function to start navigation class.
 * ***************************************************************************/
void nav::startNav(string colour){
    VideoCapture cap(imgPro.webCamNum);
    imgPro.openWebcam(cap);

    waitKey(1000);
    moveToObj(colour, cap);
}

/*moveToObj********************************************************************
 *Function for Robot to follow red object
 * ***************************************************************************/
void nav::moveToObj(string colour, VideoCapture cap){
    //Get centroid of object
    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    char dir, lastDir;
    int deadX = 25;
    int areaNoObj = 100;
    waitTime = 5;
    loop = true;
    serialCom ser;

    //While program not stopped by user
    while (loop){
        int loopCount = 0;
        int loopMax = 3;
        Point objectPT;
        imgPro.capFrame(cap, imgOut, imgOrig, colour);
        //imshow("imgBW", imgOut);
        //Point objectPT = objGeo.centre(imgBW);
        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);
        namedWindow(imgName, CV_WINDOW_AUTOSIZE);
        imshow(imgName, imgOrig);

        int x = objectPT.x;
        geometry geo;
        int area = geo.sizeLargest(imgOut, rect);
        if (area < areaNoObj){
            cout<<"Too Small"<<endl;
            cvDestroyWindow(imgName);
            findObj(colour, cap);
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
                m1Dir = '1';
                m2Dir = '1';
            }
            else if (x<0){
                dir = 'L';
                m1Speed = '0';
                m2Speed = '1';
                m1Dir = '0';
                m2Dir = '0';
            }

            //Send movement Signal to motors
            sendMove(ser, m1Speed, m2Speed, m1Dir, m2Dir);

            //If direction has changed the stop movement temporarly
            if (dir != lastDir){
                waitForSlow(500, ser);
            }
            bool stepDir = false;
            if (dir =='R' || dir == 'L' || dir == 'B' || dir == 'N'){
                stepDir = true;
            }
            if (stepDir){
                loopCount = loopMax;
            }
            if (loop && loopCount == loopMax){
                loopCount = 0;
                waitForSlow(waitTime, ser);
            }
            // Output x
            cout<<"x is: "<<x<<" Area is: "<<area<<endl;
            lastDir = dir;
        }
    }
}

/*findObj********************************************************************
 * Find Object in working area
 * ***************************************************************************/
void nav::findObj(string colour, VideoCapture cap){
    //Get centroid of object
    Rect rect;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int forwardCnt = 10;
    int turnCnt = 10;
    int areaNoObj = 100;
    waitTime = 5;
    loop = true;
    serialCom ser;
    cout<<"pre-while"<<endl;
    while(loop){
        cout<<"in-while"<<endl;
        Point objectPT;
        imgPro.capFrame(cap, imgOut, imgOrig, colour);
        //imshow("imgBW", imgOut);
        //Point objectPT = objGeo.centre(imgBW);
        objRecongition objRec;
        objRec.getBound2(imgOut, imgOrig, objectPT, rect);
        objRec.showCentre(imgOrig, objectPT);

        namedWindow("Bounding Image", CV_WINDOW_AUTOSIZE);
        imshow("Bounding Image", imgOrig);

        int x = objectPT.x;
        geometry geo;
        int area = geo.sizeLargest(imgOut, rect);
        //Area is too small--contine looking
        if (area < areaNoObj){
            cout<<"Too Small"<<endl;
            findObj(colour, cap);
        }
        //Area is big enough--proceed to object
        else{
            moveToObj(colour, cap);
        }
        //Move slightly foreward
        m1Speed = '1';
        m2Speed = '1';
        m1Dir = '0';
        m2Dir = '1';
        for (int i=0; i<forwardCnt; i++){
            sendMove(ser, m1Speed, m2Speed, m1Dir, m2Dir);
        }
        //Turn Right Slightly
        m1Speed = '1';
        m2Speed = '0';
        m1Dir = '0';
        m2Dir = '1';
        for (int i=0; i<turnCnt; i++){
            sendMove(ser, m1Speed, m2Speed, m1Dir, m2Dir);
        }
        // Output x
        cout<<"x is: "<<x<<" Area is: "<<area<<endl;
    }

}

/*waitForSlow********************************************************************
 * Delay changing directions (do not blow motors)
 * ***************************************************************************/
void nav::waitForSlow(int waitTime, serialCom ser){
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
    checkForStop(ser, waitTime, loop);
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
    int sizeSlow = 3000;
    if (a>sizeSlow && a<(sizeThresh-deadZone)) sizeRng = 'S';
    else if (a<sizeSlow) sizeRng = 'F';
    else if (a>=(sizeThresh+deadZone)) sizeRng = 'B';
    else sizeRng = 'P';
    switch (sizeRng){
    case 'F':
        dir = 'F';
        m1Dir = '0';
        m2Dir = '1';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Fast foreward"<<endl;
        break;
    case 'S':
        dir = 'N';
        m1Dir = '0';
        m2Dir = '1';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Slow foreward"<<endl;
        break;
    case 'B':
        dir = 'B';
        m1Dir = '1';
        m2Dir = '0';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Going backward"<<endl;
        break;
    case 'P':
        dir = 'S';
        m1Dir = '0';
        m2Dir = '0';
        m1Speed = '0';
        m2Speed = '0';
        cout<<"Stopped"<<endl;
        break;
    default:
        dir = 'S';
        m1Dir = '0';
        m2Dir = '0';
        m1Speed = '0';
        m2Speed = '0';
        cout<<"Default"<<endl;
        break;
    }
}

/*checkForStop********************************************************************
 * Check if stop should be done
 * ***************************************************************************/
void nav::checkForStop(serialCom ser, int time, bool& loop){
    char tmp = waitKey(time);
    if (tmp == 'c' || tmp == 'C'){
        loop = false;
        stopMovement(ser);
    }
}

/*sendMove********************************************************************
 * Sends motor movement signals to serial port
 * ***************************************************************************/
void nav::sendMove(serialCom ser, char m1Speed, char m2Speed, char m1Dir, char m2Dir){
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
    checkForStop(ser, waitTime, loop);
}

/*stopMovement********************************************************************
 * Sets the motor speed of all motors to zero (for end of the program)
 * ***************************************************************************/
void nav::stopMovement(serialCom ser){
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
    loop = false;
}



#endif // NAV_H
