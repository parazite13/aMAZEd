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

    /// Detection des murs
    vector<Point2i> coordCorner;
    coordCorner = EdgeDetection::getCorner(currentFrame);
    vector<vector<Point2i>> lines = EdgeDetection::linesDetection(currentFrame, coordCorner);

    vector<vector<Mat>> walls;

    /// Tant que les 4 coins n'ont pas été détéctées
    do{
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        transformation.getProjectionMatrix(p);
        transformation.getModelviewMatrix(m);

        walls.clear();
        vector<Mat> wall;

        /// Pour chacune des lignes
        for(const auto &line : lines) {
            
            Mat pointImageA = Mat(3, 1, CV_64FC1);
            pointImageA.at<double>(0) = line[0].x;
            pointImageA.at<double>(1) = line[0].y;
            pointImageA.at<double>(2) = 1;

            Mat pointImageB = Mat(3, 1, CV_64FC1);
            pointImageB.at<double>(0) = line[1].x;
            pointImageB.at<double>(1) = line[1].y;
            pointImageB.at<double>(2) = 1;

            Mat homography = transformation.getHomography();

            Mat pointModelA = homography * pointImageA;
            Mat pointModelB = homography * pointImageB;

            wall.push_back(pointModelA);
            wall.push_back(pointModelB);

        }

        walls.push_back(wall);

    }while(coordCorner.size() != 4);

    /// Définition des murs
    window->setWalls(walls);

    glutMaster->CallGlutMainLoop();

    return 0;
}

void loop(int){

    vector<Point2i> coordCorner;
    Mat currentFrame = cameraStream.getCurrentFrame();
    textCam = cameraStream.getCurrentFrame();
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
