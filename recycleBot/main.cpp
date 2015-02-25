#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <morphology.h>
#include <objRecognition.h>
#include <geometry.h>

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //Arduinio Device

using namespace std;
using namespace LibSerial;
using namespace cv;

// Global Declorations of Serial Information
SerialStream ardu;
const int BUFFER_SIZE = 4;
char inBuffer[BUFFER_SIZE];


void open();
void stepperMotor();
void serialTest();
void visionTest();
void motors();
void followObject();
void waitForSlow(int);
void tmp();

int main(){
    waitKey(10000);
    //followObject();
    tmp();
}

void tmp(){
     objRecongition objRec;
     objRec.getColourRange();
}

void followObject(){
    //Get centroid of object
    Mat imgOrig, imgHSV, imgOut;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    char m1Speed, m2Speed, m1Dir, m2Dir;
    int webCamNum = 1;

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
        cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV); //BGR to HSV
        inRange(imgHSV, Scalar(LHue, LSat, LVal), Scalar(HHue, HSat, HVal), imgOut);
        blur(imgOut, imgOut, Size(3,3));
        //imshow("inRange", imgOut);
        morphology objMor;
        geometry objGeo;
        Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(6, 6)); //Set structuing element to be a 5x5 circle
        imgOut = objMor.clo(imgOut,SE);
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
        //If approximately in middle then do not move motors
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
        open();
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

void waitForSlow(int waitTime){
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

void serialTest(){
    open();
    stepperMotor();
}

void stepperMotor(){
    int stepNum;
    cout<<"Input number of Steps:"<<endl;
    cin>>stepNum;
    ardu.write(inBuffer, BUFFER_SIZE);
}

void motors(){
    char m1Speed, m2Speed, m1Dir, m2Dir;
    cout<<"Input Motor1 Data:\nSpeed:";
    cin>>m1Speed;
    cout<<"Direction:";
    cin>>m1Dir;
    cout<<"Input Motor2 Data:\nSpeed:";
    cin>>m2Speed;
    cout<<"Direction:";
    cin>>m2Dir;

    inBuffer[0] = m1Speed;
    inBuffer[1] = m1Dir;
    inBuffer[2] = m2Speed;
    inBuffer[3] = m2Dir;

    open();
    ardu.write(inBuffer, BUFFER_SIZE);
    //close Serial Port
    ardu.Close();


    //ardu.write(inBuffer, BUFFER_SIZE);

}

void open(){
    /*The arduino must be setup to use the same baud rate*/
    ardu.Open(PORT);
    ardu.SetBaudRate(SerialStreamBuf::BAUD_9600);
    ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
}

void visionTest(){
    Mat imgOrig, imgHSV, imgOut;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    VideoCapture cap(1); //capture the video from web cam

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
        imshow("Original Image", imgOrig);
        cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV); //BGR to HSV
        inRange(imgHSV, Scalar(LHue, LSat, LVal), Scalar(HHue, HSat, HVal), imgOut);
        blur(imgOut, imgOut, Size(3,3));
        imshow("inRange", imgOut);
        morphology objMor;
        geometry objGeo;
        Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(6, 6)); //Set structuing element to be a 5x5 circle
        imgOut = objMor.clo(imgOut,SE);
        imshow("mor", imgOut);
        Point objectPT = objGeo.centre(imgOut);
        objRec.showCentre(imgOrig, objectPT);
        objRec.getBound(imgOut, imgOrig);

        //imshow("Output Image", imgOut);
        imshow("Bound and Centre", imgOrig);
        if (waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
}


