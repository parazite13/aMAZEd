#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
 {

     VideoCapture capture(0); // open the default camera
     if(!capture.isOpened())  // check if we succeeded
         return -1;

     Mat edges;
     namedWindow("Capture",1);
     while(true)
     {
         Mat frame;
         capture >> frame; // get a new frame from camera
         
         imshow("Capture", frame);

         // Quitte l'application lorsqu'on appuie sur Echap
         if(waitKey(30) == 27) break;
     }

     // the camera will be deinitialized automatically in VideoCapture destructor
     return 0;

}