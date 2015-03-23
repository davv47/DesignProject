#ifndef OBJRECOGNITION_H
#define OBJRECOGNITION_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include <geometry.h>

using namespace std;
using namespace cv;

class objRecongition{
    public:
    objRecongition();
    ~objRecongition();
    void getColour(int&, int&, int&, int&, int&, int&, string);
    void getBound(Mat&, Mat&);
    void getBound2(Mat&, Mat&, Point&, Rect&);
    void showCentre(Mat&, Point);
    void getColorRange();
};

#endif // OBJRECOGNITION_H
