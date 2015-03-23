#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

class geometry{
    public:
        geometry();
        ~geometry();
        double PI;
        Mat _outIM, _origIM, element;
        Point _centre;
        double _angle;
        int _quad;
        string _getTime;
        int _size;

        Mat rot(Mat, int);
        Mat crop(Mat, int, int);
        Mat cropPt(Mat, Rect);
        Point centre(Mat);
        double angle(Mat);
        int quad(Mat, Point);
        void showCentre(Mat&, Point);
        int size(Mat);
        int sizeLargest(Mat, Rect);
        void imFlip(Mat&, Mat&);
        Point getCentreOfRect(Rect);

};


#endif // GEOMETRY_H
