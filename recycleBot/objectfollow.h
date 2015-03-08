#ifndef OBJECTFOLLOW
#define OBJECTFOLLOW

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

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //ser.arduinio Device

using namespace std;
using namespace cv;

class objFollow{
    public:
    void followObj(string);
    void waitForSlow(int, serialCom, bool&);
    void moveLine(int, char&, char&, char&, char&, char&);
    void stopMovement(serialCom);
};
/*followObj********************************************************************
 *Function for Robot to follow red object
 * ***************************************************************************/
void objFollow::followObj(string colour){
    //Get centroid of object
    Mat imgOrig, imgHSV, imgOut;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int webCamNum = 1;
    int xRez = 640/2;
    int yRez = 480/2;
    int deadX = 25;
    char dir, lastDir;
    bool loop = true;

    serialCom ser;

    VideoCapture cap(webCamNum); //capture the video from web cam

    if (!cap.isOpened()){
         cout << "Cannot open the web cam" << endl;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH,xRez);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,yRez);
    //Get Object to detect
    cap.read(imgOrig);
    objRecongition objRec;
    objRec.getColour(LHue, HHue, LSat, HSat, LVal, HVal, colour);
    cout<<LHue<<endl;

    waitKey(1000);
    //While program not stopped by user
    while (loop){
        int loopCount = 0;
        int loopMax = 3;
        cap.read(imgOrig); //get video frame
        //imshow("Original Image", imgOrig);
        morphology objMor;
        geometry objGeo;
        Point objectPT;
        cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV); //BGR to HSV
        inRange(imgHSV, Scalar(LHue, LSat, LVal), Scalar(HHue, HSat, HVal), imgOut);
        blur(imgOut, imgOut, Size(3,3));
        imshow("inRange", imgOut);
        Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(2, 2)); //Set structuing element to be a 5x5 circle
        imgOut = objMor.clo(imgOut,SE);
        int area = objGeo.size(imgOut);
        imshow("mor", imgOut);
        //Point objectPT = objGeo.centre(imgOut);
        objRec.getBound2(imgOut, imgOrig, objectPT);
        objRec.showCentre(imgOrig, objectPT);

        //imshow("Output Image", imgOut);
        imshow("Bound and Centre", imgOrig);

        int x = objectPT.x;

        //If no object in frame treat object as in centre of window
        if (x<0){
            x = xRez/2-1;
        }
        x = x-xRez/2-1;

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

        //If direction has changed the stop movement temporarly
        if (dir != lastDir){
            waitForSlow(500, ser, loop);
        }
        if (loop){
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
            //Delay to account for small move & check for end program signal
            char tmp = waitKey(5);
            if (tmp == 'c' || tmp == 'C'){
                stopMovement(ser);
                loop = false;
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
                waitForSlow(5, ser, loop);
            }
            // Output x
            cout<<"x is: "<<x<<" Area is: "<<area<<endl;
            lastDir = dir;

        }
    }
}
/*waitForSlow********************************************************************
 * Delay changing directions (do not blow motors)
 * ***************************************************************************/
void objFollow::waitForSlow(int waitTime, serialCom ser, bool& loop){
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
    char tmp = waitKey(waitTime);
    if (tmp == 'c' || tmp =='C'){
        stopMovement(ser);
        loop = false;
    }
}

/*moveLine********************************************************************
 * Move object forward or backword based on its size
 * Input:
 *      a - area of object in image
 * Modifies:
 *      -Motor Speeds
 *      -Motor Directions
 * ***************************************************************************/
void objFollow::moveLine(int a, char& m1Speed, char& m2Speed, char& m1Dir, char& m2Dir, char& dir){
    int sizeThresh = 10000;
    int deadZone = 2000;
    int sizeSlow = 500;
    if (a < (sizeThresh-deadZone)){
        dir = 'F';
        m1Dir = '0';
        m2Dir = '1';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Going foreward"<<endl;
    }
    else if(a > (sizeThresh+deadZone)){
        dir = 'B';
        m1Dir = '1';
        m2Dir = '0';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Going backward"<<endl;
    }
    else if(a > sizeSlow){
        dir = 'N';
        m1Dir = '0';
        m2Dir = '1';
        m1Speed = '1';
        m2Speed = '1';
        cout<<"Going foreward"<<endl;
    }
    else{
        dir = 'S';
        m1Dir = '0';
        m2Dir = '0';
        m1Speed = '0';
        m2Speed = '0';
        cout<<"Stopped"<<endl;
    }
}

/*stopMovement********************************************************************
 * Sets the motor speed of all motors to zero (for end of the program)
 * ***************************************************************************/
void objFollow::stopMovement(serialCom ser){
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
}

#endif // OBJECTFOLLOW

