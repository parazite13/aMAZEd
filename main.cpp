#include "modelisation/OpenGL.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"

using namespace cv;
using namespace std;

CameraStream *cameraStream = nullptr;
GlutMaster *glutMaster = nullptr;
OpenGL *window = nullptr;

/// Textures
Mat textCam;
Mat textMaze;

/// Matrices de projection et de modelview
double p[16];
double m[16];

void loop(int);
void setupMaze();

int main(int argc, char** argv){

    cameraStream = new CameraStream();
    namedWindow("aMAZEd Calibration");

    while(true){

        Mat currentFrame = cameraStream->getCurrentFrame();
        putText(currentFrame, "Press space bar to start calibration", Point2i(0, currentFrame.rows), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);

        imshow("aMAZEd Calibration", currentFrame);

        /// Si on appuie sur la touche espace
        if(waitKey(30) == 32) break;
    }

    setupMaze();

    glutMaster->CallGlutMainLoop();
    delete cameraStream;

    return 0;
}

void loop(int){

    EdgeDetection edgeDetection = EdgeDetection(cameraStream);

    vector<Point2i> coordCorner;
    Mat currentFrame = cameraStream->getCurrentFrame();
    textCam = cameraStream->getCurrentFrame();
    coordCorner = edgeDetection.getCorner(currentFrame);
    edgeDetection.linesDetection(currentFrame, coordCorner);

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4){
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        transformation.getProjectionMatrix(p);
        transformation.getModelviewMatrix(m);
    }

    glutPostRedisplay();

}

void setupMaze(){

    /// Calibration des couleurs
    EdgeDetection edgeDetection = EdgeDetection(cameraStream);
    edgeDetection.colorCalibration();

    Mat currentFrame = cameraStream->getCurrentFrame();
    double ratio = (double)currentFrame.cols / (double)currentFrame.rows;
    int width = 1000; //largeur de la fenêtre

    glutMaster = new GlutMaster();
    window = new OpenGL(glutMaster, width, (int)(width / ratio), 0, 0, (char*)("aMAZEd"));

    /// Detection des murs
    vector<vector<Mat>> walls;
    vector<Point2i> coordCorner;
    coordCorner = edgeDetection.getCorner(currentFrame);
    vector<vector<Point2i>> lines = edgeDetection.linesDetection(currentFrame, coordCorner);

    /// Tant que les 4 coins n'ont pas été détéctées
    do{
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);

        walls.clear();
        vector<Mat> wall;

        /// Pour chacune des lignes
        for(const auto &line : lines) {

            wall.clear();

            Mat pointImageA = Mat(3, 1, CV_64FC1);
            pointImageA.at<double>(0) = line[0].x;
            pointImageA.at<double>(1) = line[0].y;
            pointImageA.at<double>(2) = 1;

            Mat pointImageB = Mat(3, 1, CV_64FC1);
            pointImageB.at<double>(0) = line[1].x;
            pointImageB.at<double>(1) = line[1].y;
            pointImageB.at<double>(2) = 1;

            Mat homography = transformation.getHomography();

            /// Calcul des coordonées des murs sur le modele
            Mat pointModelA = homography * pointImageA;
            Mat pointModelB = homography * pointImageB;

            wall.push_back(pointModelA);
            wall.push_back(pointModelB);

            walls.push_back(wall);

        }

    }while(coordCorner.size() != 4);

    /// Définition des murs
    if(!walls.empty()){
        window->setWalls(walls);
    }

}