#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>


using namespace std;
using namespace cv;


class morphology {
private:

public:
    double PI;

    morphology();
    ~morphology();

    Mat _outIM, _origIM, element;
    Mat dil(Mat, Mat);
    Mat ero(Mat, Mat);
    Mat ope(Mat, Mat);
    Mat clo(Mat, Mat);

};
#endif // MORPHOLOGY_H
