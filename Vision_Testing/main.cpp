#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <morphology.h>
#include <objRecognition.h>
#include <geometry.h>




using namespace cv;
using namespace std;

 int main() {
    Mat imgOrig, imgHSV, imgOut;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    VideoCapture cap(1); //capture the video from web cam

    if (!cap.isOpened()){
         cout << "Cannot open the web cam" << endl;
         return -1;
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

    return 0;

}
