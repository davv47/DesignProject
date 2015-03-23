#ifndef IMGPROCESS
#define IMGPROCESS

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>

using namespace std;
using namespace cv;

class imgProcess{
    public:
        int webCamNum;
        int xRez, yRez;
        imgProcess();
        ~imgProcess();
        void openWebcam(VideoCapture&);
        void capFrame(VideoCapture, Mat&, Mat&, string);
};


#endif // IMGPROCESS

