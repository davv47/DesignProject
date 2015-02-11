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
    Mat rot(Mat, int);
    Mat crop(Mat, int, int);
    Point centre(Mat);
    double angle(Mat);
    int quad(Mat, Point);
    string getTime(double, int, int);

};
Mat morphology::dil(Mat modIM, Mat ele){
   dilate(modIM, _outIM, ele, Point(-1, -1), 2, 1, 1);
   return _outIM;
}

Mat morphology::ero(Mat modIM, Mat ele){
   erode(modIM, _outIM, ele, Point(-1, -1), 2, 1, 1);
   return _outIM;
}

Mat morphology::ope(Mat modIM, Mat ele){
    ero(modIM, ele);
    dil(modIM, ele);
    return _outIM;
}

Mat morphology::clo(Mat modIM, Mat ele){
    dil(modIM, ele);
    ero(modIM, ele);
    return _outIM;
}

Mat morphology::rot(Mat INimg, int angle){
    int size = max(INimg.cols, INimg.rows);
    Point2f pt(size/2., size/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(INimg, _outIM, r, Size(size, size));
    stringstream ss;
    ss << angle;
    //cout << ss.str() << endl;
    return _outIM;
}

Mat morphology::crop(Mat INimg, int newRows, int newCols){
    int origRows = INimg.rows;
    int origCols = INimg.cols;
    int x = (origRows - newRows)/2;
    int y = (origCols - newCols)/2;
    int h = newRows;
    int w = newCols;
    _outIM = INimg(Rect(x,y,h,w));
    return _outIM;
}

Point morphology::centre(Mat INimg){
    Point pt;
    Mat TMPimg = INimg;
    //bitwise_not(TMPimg,TMPimg);
    Moments mm = moments(TMPimg,true);
    pt.x = int(mm.m10 / mm.m00);
    pt.y = int(mm.m01 / mm.m00);
    //imshow("centre",INimg);

    return pt;

}

double morphology::angle(Mat INimg){
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
    theta = atan2(2*uP11, uP20-uP02)*180/(2*PI)+180;

    return 180 - theta;
}

int morphology::quad(Mat INimg, Point objPT){
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

    return quadNum;
}

string morphology::getTime(double theta, int quad, int delta){
    string time, h, m;
    int INTtheta = theta;
    int hour;
    if (quad==1){   //fix to be more dynamic
        for (int i=0; i<=90/delta; i++){
            if (INTtheta >= 0 && INTtheta < delta/2){
                hour = 3;
                cout<<"okay"<<endl;
            }
            else if (INTtheta>=delta/2 && INTtheta<delta+delta/2){
                hour = 2;
            }
            else if (INTtheta>=delta+delta/2 && INTtheta<2*delta+delta/2){
                hour = 1;
            }
            else if(INTtheta>=2*delta+delta/2 && INTtheta<=3*delta){
                hour = 12;
            }
        }
    }
    else if (quad==2){
        hour = INTtheta/30+10;
    }
    else if (quad==3){
        hour = INTtheta/30+7;
    }
    else if (quad==4){
        hour = INTtheta/30+4;
    }
    int min = 0; //correct to greater percision later
    stringstream out1;
    stringstream out2;
    out1<<hour;
    h = out1.str();
    out2<<min;
    m = out2.str();
    time = h+":"+m+"0";
    if (hour!=12 && hour!=0){
        cout<<INTtheta<<endl;
        cout<<"The time is "<<time<<endl;
    }
    return time;
}

#endif // MORPHOLOGY_H
#ifndef MOROHOLOGY_H
#define MOROHOLOGY_H

#endif // MOROHOLOGY_H
