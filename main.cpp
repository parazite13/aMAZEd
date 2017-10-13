#include "modelisation/OpenGL.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"
#include "modelisation/GlutMaster.h"

using namespace cv;
using namespace std;


GlutMaster *glutMaster;
OpenGL *window = 0;

CameraStream cameraStream = CameraStream();

/// Textures
Mat textCam;
Mat textMaze;

/// Matrices de projection et de modelview
double p[16];
double m[16];

int main(int argc, char** argv){
    Mat currentFrame = cameraStream.getCurrentFrame();
    double ratio = (double)currentFrame.cols / (double)currentFrame.rows;
    int width = 1280; //largeur de la fenêtre
    EdgeDetection::colorCalibration();

    glutMaster = new GlutMaster();
    window = new OpenGL(glutMaster, width, (int)(width / ratio), 0, 0, (char*)("aMAZEd"));

    glutMaster->CallGlutMainLoop();

    return 0;
}

void loop(int){

    vector<Point2i> coordCorner;
    Mat currentFrame = cameraStream.getCurrentFrame();
    textCam = cameraStream.getCurrentFrame();
    coordCorner = EdgeDetection::getCorner(currentFrame);
    vector<vector<Point2i>> lines = EdgeDetection::linesDetection(currentFrame, coordCorner);

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4){
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        transformation.getProjectionMatrix(p);
        transformation.getModelviewMatrix(m);

        window->setHomography(transformation.getHomography());
        window->setLines(lines);
    }

    glutPostRedisplay();

}
