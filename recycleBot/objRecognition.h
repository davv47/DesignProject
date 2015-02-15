#ifndef OBJRECOGNITION_H
#define OBJRECOGNITION_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>

using namespace std;
using namespace cv;

class objRecongition{
    public:
    void getColour(Mat&, int&, int&, int&, int&, int&, int&);
    void getBound(Mat&, Mat&);
    void showCentre(Mat&, Point);
};

void objRecongition::getColour(Mat &imgOrig, int &LHue, int &HHue, int &LSat, int &HSat, int &LVal, int &HVal){
    //Red       0   -   59
    //Yellow    60  -   119
    //Green     120 -   179
    //Cyan      180 -   239
    //Blue      240 -   299
    //Magenta   300 -   359
    string colour;
    colour = "red";
    if(colour == "red"){
        LHue = 0;
        HHue = 179;
    }

    if(colour == "blue"){ //not working yet
        LHue = 179;
        HHue = 360;
    }

    LSat = 145;
    HSat = 245;

    LVal = 100;
    HVal = 255;

    if(0){
    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    cvCreateTrackbar("LowH", "Control", &LHue, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &HHue, 179);

    cvCreateTrackbar("LowS", "Control", &LSat, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &HSat, 255);

    cvCreateTrackbar("LowV", "Control", &LVal, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &HVal, 255);
    }

}
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

void objRecongition::showCentre(Mat &imgOut, Point objectPT){
    int thickness = -1;
    int lineType = 8;
    circle (imgOut, objectPT, 5, Scalar(0,0,0), thickness, lineType);
}


#endif // OBJRECOGNITION_H
