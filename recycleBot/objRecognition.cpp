#include "objRecognition.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include <geometry.h>

using namespace std;
using namespace cv;

objRecongition::objRecongition(){}
objRecongition::~objRecongition(){}

/*getColour********************************************************************
 *returns the HSV values of the colour of the object selected
 * Inputs:
 *      -colour: the colour of the object
 * Modifies:
 *      -HSV values
 * ***************************************************************************/
void objRecongition::getColour(int &LHue, int &HHue, int &LSat, int &HSat, int &LVal, int &HVal, string colour){
    /*Red       0   -   59
    //Yellow    60  -   119
    //Green     120 -   179
    //Cyan      180 -   239
    //Blue      240 -   299
    Magenta   300 -   359*/

    if(colour == "red"){
        //LHue = 0;
        //HHue = 12;
        LHue = 125;
        HHue = 179;

        LSat = 125;
        HSat = 255;

        LVal = 0;
        HVal = 255;
    }
    else if(colour == "blue"){
        LHue = 105;
        HHue =135;

        LSat = 167;
        HSat = 255;

        LVal = 0;
        HVal = 255;
    }

}

/*getBound********************************************************************
 *Shows all bounding boxes of all objects in frame that meet HSV
 *requirements
 * ***************************************************************************/
void objRecongition::getBound(Mat &imgOut, Mat &imgOrig){
    vector <vector <Point> > contours;
    vector <Vec4i> hierarchy;
    RNG rng(12345);
    findContours(imgOut, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector <vector <Point> >contours_poly(contours.size());
    vector<Rect> boundRect(contours.size() );


    for( int i = 0; i < contours.size(); i++ ){
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect(Mat(contours_poly[i]) );
    }
    for( int i = 0; i< contours.size(); i++ ){
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        rectangle(imgOrig, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
    }

}

/*getBound2********************************************************************
 *Shows bounds of largest object that meets HSV requirements
 * ***************************************************************************/
void objRecongition::getBound2(Mat &imgOut, Mat &imgOrig, Point& pt, Rect& bounding_rect){
    vector <vector <Point> > contours;
    vector <Vec4i> hierarchy;
    int largest_area=0;
     int largest_contour_index=0;
    findContours(imgOut, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector <vector <Point> >contours_poly(contours.size());
    vector<Rect> boundRect(contours.size() );

    for( int i = 0; i< contours.size(); i++ ){
        double a =contourArea(contours[i]);
        if(a>largest_area){
               largest_area=a;
               largest_contour_index=i;                //Store the index of largest contour
        }
        bounding_rect = boundingRect(contours[largest_contour_index]);

        //Get centre of bounding rectangle
        geometry geo;
        pt = geo.getCentreOfRect(bounding_rect);

        Scalar color = Scalar(0, 0, 0);
        rectangle(imgOrig, bounding_rect, color, 2, 8, 0 );
    }
}

/*getColour********************************************************************
 *Puts centre point on image passed to function
 * Inputs:
 *      -Point of object centroid
 * Modifies:
 *      -imgOut to show position of this object
 * ***************************************************************************/
void objRecongition::showCentre(Mat &imgOut, Point objectPT){
    int thickness = -1;
    int lineType = 8;
    circle (imgOut, objectPT, 5, Scalar(0,0,0), thickness, lineType);
}

/*getColorRange********************************************************************
 *Function to play with colour values and see what is detected
 * ***************************************************************************/
void objRecongition::getColorRange(){

    VideoCapture cap(1); //capture the video from web cam

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  int iLowH = 0;
 int iHighH = 179;

  int iLowS = 0;
 int iHighS = 255;

  int iLowV = 0;
 int iHighV = 255;

  //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

while (true){
    Mat imgOriginal;

    bool bSuccess = cap.read(imgOriginal); // read a new frame from video

     if (!bSuccess) //if not success, break loop
    {
         cout << "Cannot read a frame from video stream" << endl;
         break;
    }

  Mat imgHSV;

  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  Mat imgThresholded;

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

  //morphological opening (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //morphological closing (fill small holes in the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   imshow("Thresholded Image", imgThresholded); //show the thresholded image
  imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }
}


