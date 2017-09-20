#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
 {

     VideoCapture cap(0); // open the default camera
     if(!cap.isOpened())  // check if we succeeded
         return -1;

     Mat edges;
     namedWindow("edges",1);
     while(true)
     {
         Mat frame;
         cap >> frame; // get a new frame from camera
         //cvtColor(frame, edges, CV_BGR2GRAY);
         //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
         //Canny(edges, edges, 0, 30, 3);
         imshow("edges", frame);
         waitKey(30);
     }

     // the camera will be deinitialized automatically in VideoCapture destructor
     return 0;

}