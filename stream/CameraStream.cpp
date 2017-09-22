#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#include "CameraStream.h"

CameraStream::CameraStream(){
    this->capture = VideoCapture(0);
}

bool CameraStream::isOpen(){
   return this->capture.isOpened();
}

void CameraStream::showCamera() {
    namedWindow("Capture",cv::WINDOW_AUTOSIZE);
    Mat currentFrame;
    while(true){
        this->capture >> currentFrame; // get a new frame from camera
        imshow("Capture", currentFrame);

        // Quitte l'application lorsqu'on appuie sur Echap
        if(waitKey(30) == 27) break;
    }
}

Mat CameraStream::getCurrentFrame() {
    Mat currentFrame;
    this->capture >> currentFrame; // get a new frame from camera
    Mat frameFlip;
    flip(currentFrame, frameFlip, 1); // 1 -> flip axe y (0 -> axe x)
    return frameFlip;
}