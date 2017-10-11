#include "modelisation/OpenGL.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"

using namespace cv;
using namespace std;

CameraStream cameraStream = CameraStream();

/// Textures
Mat textCam;
Mat textMaze;

/// Matrices de projection et de modelview
double p[16];
double m[16];

int main(int argc, char** argv){

    OpenGL::init(argc, argv);

    return 0;
}

void loop(int){

    vector<Point2d> coordCorner;
    Mat currentFrame = cameraStream.getCurrentFrame();
//    Mat currentFrame = imread("toto.png");
    textCam = currentFrame;
    coordCorner = EdgeDetection::getCorner(currentFrame);
    EdgeDetection::linesDetection(currentFrame, coordCorner);

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4){
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        transformation.getProjectionMatrix(p);
        transformation.getModelviewMatrix(m);
    }

    glutPostRedisplay();

}
