#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <morphology.h>
#include <objRecognition.h>
#include <geometry.h>
#include <objectfollow.h>
#include "nav.h"

#include <SerialStream.h>
#define PORT "/dev/ttyACM0" //Arduinio Device
using namespace std;
using namespace LibSerial;
using namespace cv;


serialCom ser;

// Global Declorations of Serial Information

int main(){
    nav run;
    /*string colour = "red";
    waitKey(10000);
    run.startNav(colour);*/
    run.closeMove();
}

