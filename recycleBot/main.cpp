#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>

#include "nav.h"
#include <SerialStream.h>

#define PORT "/dev/ttyACM0" //Arduinio Device
using namespace std;
using namespace LibSerial;
using namespace cv;

int main(){
    nav run;
    string colour = "red";
    waitKey(10000);
    run.startNav(colour);
}

