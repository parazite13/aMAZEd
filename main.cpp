#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: aMAZEd ImageToLoadAndDisplay" << endl;
        return -1;
    }

    Mat frame;
    frame = imread(argv[1], IMREAD_COLOR); // Read the file

    if(!frame.data) // Check for invalid input
    {
        cout << "Could not open or find the frame" << endl;
        return -1;
    }
    

    namedWindow("Window", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Window", frame); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}