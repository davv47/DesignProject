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
    void followObj();
    void waitForSlow(int);
    void followLine(int);
};

void objFollow::followObj(){
    //Get centroid of object
    Mat imgOrig, imgHSV, imgOut;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int webCamNum = 1;

    serialCom ser;

    VideoCapture cap(webCamNum); //capture the video from web cam

    if (!cap.isOpened()){
         cout << "Cannot open the web cam" << endl;
    }

    //Get Object to detect
    cap.read(imgOrig);
    objRecongition objRec;
    objRec.getColour(imgOrig, LHue, HHue, LSat, HSat, LVal, HVal);
    cout<<LHue<<endl;


    while (true){
        cap.read(imgOrig); //get video frame
        //imshow("Original Image", imgOrig);
        morphology objMor;
        geometry objGeo;
        cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV); //BGR to HSV
        inRange(imgHSV, Scalar(LHue, LSat, LVal), Scalar(HHue, HSat, HVal), imgOut);
        blur(imgOut, imgOut, Size(3,3));
        //imshow("inRange", imgOut);
        Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(6, 6)); //Set structuing element to be a 5x5 circle
        imgOut = objMor.clo(imgOut,SE);
        int area = objGeo.size(imgOut);
        //imshow("mor", imgOut);
        Point objectPT = objGeo.centre(imgOut);
        objRec.showCentre(imgOrig, objectPT);
        objRec.getBound2(imgOut, imgOrig);

        //imshow("Output Image", imgOut);
        imshow("Bound and Centre", imgOrig);

        int x = objectPT.x;

        //If no object in frame treat object as in centre of window
        if (x<0){
            x = 319;
        }
        x = x-319;

        //Move Motors
        //If approximately in middle then move in straight line
        if (abs(x) < 50){

            x = 0;
            m1Speed = '0';
            m2Speed = '0';
        }
        // Otherwise move motors at half speed
        else{
            m1Speed = '5';
            m2Speed = '5';
        }

        // If in left of frame move motors Dir 1
        if(x>0){
            if (m1Dir == '0'){
                waitForSlow(1000);
            }
            m1Dir = '1';
            m2Dir = '1';
        }
        else{
            if (m1Dir == '1'){
                waitForSlow(1000);
            }
            m1Dir = '0';
            m2Dir = '0';
        }

        //Send Signal to motors
        inBuffer[0] = m1Speed;
        inBuffer[1] = m1Dir;
        inBuffer[2] = m2Speed;
        inBuffer[3] = m2Dir;

        //Open Serial Port
        ser.open();
        //Send data
        /*for (int i = 0; i<4; i++){
            ardu << inBuffer[i];
        }*/
        ardu.write(inBuffer, BUFFER_SIZE);
        //close Serial Port
        ardu.Close();

        //Delay to account for small move
        waitKey(10);

        // Output x
        cout<<x<<endl;
    }
}

void objFollow::waitForSlow(int waitTime){
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
    waitKey(waitTime);

}

/*moveLine********************************************************************
 * Move object forward or backword based on its size
 * ***************************************************************************/
void objFollow::moveLine(int a){
    int sizeThresh = 50;//??
    if (a < (sizeThresh-deadZone)){
        //Move Foreward
    }
    else if( a > (sizeThresh+deadZone)){
        //Move backwards
    }
}

#endif // OBJECTFOLLOW

