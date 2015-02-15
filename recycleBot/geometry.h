#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>

using namespace std;
using namespace cv;

class geometry{
    public:
        double PI = 4*atan(1);
        Mat _outIM, _origIM, element;
        Point _centre;
        double _angle;
        int _quad;
        string _getTime;
        int _size;

        Mat rot(Mat, int);
        Mat crop(Mat, int, int);
        Point centre(Mat);
        double angle(Mat);
        int quad(Mat, Point);
        void showCentre(Mat&, Point);
        int size(Mat);
        void imFlip(Mat&, Mat&);
};



Mat geometry::rot(Mat INimg, int angle){
    int size = max(INimg.cols, INimg.rows);
    Point2f pt(size/2., size/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(INimg, _outIM, r, Size(size, size));
    stringstream ss;
    ss << angle;
    //cout << ss.str() << endl;
    return _outIM;
}

Mat geometry::crop(Mat INimg, int newRows, int newCols){
    int origRows = INimg.rows;
    int origCols = INimg.cols;
    int x = (origRows - newRows)/2;
    int y = (origCols - newCols)/2;
    int h = newRows;
    int w = newCols;
    _outIM = INimg(Rect(x,y,h,w));
    return _outIM;
}

Point geometry::centre(Mat INimg){
    Point pt;
    Mat TMPimg = INimg;
    Moments mm = moments(TMPimg,true);
    pt.x = int(mm.m10 / mm.m00);
    pt.y = int(mm.m01 / mm.m00);
    _centre = pt;
    return _centre;

}

int geometry::size(Mat INimg){
    Moments mm = moments(INimg, true);
    _size = mm.m00;
    return _size;
}

//Returns angle between 0 and 180 degrees
double geometry::angle(Mat INimg){
    double theta;
    Mat TMPimg = INimg;
    //bitwise_not(TMPimg,TMPimg);
    Moments mm = moments(TMPimg,true);
    double u00 = mm.m00;
    double u11 = mm.mu11;
    double u20 = mm.mu20;
    double u02 = mm.mu02;
    double uP20 = u20/u00;
    double uP02 = u02/u00;
    double uP11 = u11/u00;
    theta = atan2(2*uP11, uP20-uP02)*180/(2*PI);
    if (theta <0){
        theta = theta+360;
    }
    if (theta>180){
        theta = theta-180;
    }
    if (theta>90){
        theta = theta-90;
    }
    _angle = theta;
    return _angle;
}

int geometry::quad(Mat INimg, Point objPT){
    int quadNum;
    int centreRows = INimg.rows/2;
    int centreCols = INimg.cols/2;
    int xVal = objPT.x-centreCols;  //X starts ar "left" of image
    int yVal = centreRows-objPT.y;  //Y starts at "top" of image
    if (xVal+centreCols<0 || yVal+centreRows<0){
        quadNum = 0;
    }
    else if(xVal>=0 && yVal>=0){
        quadNum = 1;
    }
    else if(xVal<0 && yVal>=0){
        quadNum = 2;
    }
    else if (xVal<0 && yVal<0){
        quadNum = 3;
    }
    else if (xVal>=0 && yVal<0){
        quadNum = 4;
    }
    else{
        quadNum = 0;
    }

    _quad = quadNum;
    return _quad;
}

void geometry::imFlip(Mat &INimg, Mat& OUTimg){
    flip(INimg, OUTimg, 1);
}


void geometry::showCentre(Mat &imgOut, Point objectPT){
    int thickness = -1;
    int lineType = 8;
    circle (imgOut, objectPT, 20, Scalar(255,0,0), thickness, lineType);
}

#endif // GEOMETRY_H
