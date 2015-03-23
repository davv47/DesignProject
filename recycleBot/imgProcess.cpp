#include "imgProcess.h"

#include <string.h>
#include <cmath>

#include "geometry.h"
#include "morphology.h"
#include "objRecognition.h"

using namespace std;
using namespace cv;

/*imgProcess********************************************************************
 *Constructor for imgProcess class
 * ***************************************************************************/
imgProcess::imgProcess(){
    webCamNum = 1;
    xRez = 640/2;
    yRez = 480/2;
}

/*imgProcess********************************************************************
 *Desstructor for imgProcess class
 * ***************************************************************************/
imgProcess::~imgProcess(){
}


/*geometry********************************************************************
 *Constructor for Geometry class
 * ***************************************************************************/
void imgProcess::openWebcam(VideoCapture& cap){
    if (!cap.isOpened()){
         cout << "Cannot open the web cam" << endl;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH,xRez);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,yRez);
}

/*geometry********************************************************************
 *Constructor for Geometry class
 * ***************************************************************************/
void imgProcess::capFrame(VideoCapture cap, Mat& imgBW, Mat& imgOrig, string colour){
    Mat imgHSV;
    int LHue, HHue, LSat, HSat, LVal, HVal;
    cap.read(imgOrig); //get video frame
    //imshow("Original Image", imgOrig);
    morphology objMor;
    cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV); //BGR to HSV
    objRecongition objRec;
    objRec.getColour(LHue, HHue, LSat, HSat, LVal, HVal, colour);
    inRange(imgHSV, Scalar(LHue, LSat, LVal), Scalar(HHue, HSat, HVal), imgBW);
    blur(imgBW, imgBW, Size(3,3));
    //imshow("inRange", imgBW);
    Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(2, 2)); //Set structuing element to be a 5x5 circle
    imgBW = objMor.clo(imgBW,SE);
    //imshow("mor", imgBW);

    //imshow("Output Image", imgBW);
    //imshow("Bound and Centre", imgOrig);
}
