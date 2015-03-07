#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <cmath>

using namespace std;
using namespace cv;


class morphology {
private:

public:
    double PI = 4*atan(1);
    Mat _outIM, _origIM, element;
    Mat dil(Mat, Mat);
    Mat ero(Mat, Mat);
    Mat ope(Mat, Mat);
    Mat clo(Mat, Mat);

};
/*dil********************************************************************
 *Dilates specified image based on specified structuring element
 * ***************************************************************************/
Mat morphology::dil(Mat modIM, Mat ele){
   dilate(modIM, _outIM, ele, Point(-1, -1), 2, 1, 1);
   return _outIM;
}

/*ero********************************************************************
 *erodesspecified image based on specified structuring element
 * ***************************************************************************/
Mat morphology::ero(Mat modIM, Mat ele){
   erode(modIM, _outIM, ele, Point(-1, -1), 2, 1, 1);
   return _outIM;
}

/*ope********************************************************************
 *Opens specified image based on specified structuring element
 * ***************************************************************************/
Mat morphology::ope(Mat modIM, Mat ele){
    ero(modIM, ele);
    dil(modIM, ele);
    return _outIM;
}

/*clo********************************************************************
 *Closes specified image based on specified structuring element
 * ***************************************************************************/
Mat morphology::clo(Mat modIM, Mat ele){
    dil(modIM, ele);
    ero(modIM, ele);
    return _outIM;
}

#endif // MORPHOLOGY_H
