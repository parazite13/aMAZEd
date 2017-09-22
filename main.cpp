#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "stream/CameraStream.h"
#include "analyse/EdgeDetection.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    CameraStream fluxCamera = CameraStream();
    if(!fluxCamera.isOpen())
        return -1;

   // fluxCamera.showCamera();
    Mat frame;
    while (true){
        frame = fluxCamera.getCurrentFrame();
        EdgeDetection::cornersDetection(frame, 200);
        namedWindow("Frame",WINDOW_AUTOSIZE);
        imshow("Frame", frame);
        if(waitKey(30) == 27) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;

}