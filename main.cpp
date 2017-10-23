#include "modelisation/OpenGL.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"
#include "physics/AngleModel.h"

using namespace cv;
using namespace std;

CameraStream *cameraStream = nullptr;
OpenGL *window = nullptr;
AngleModel *angleModel = nullptr;
Ball *ball = nullptr;

/// Prototypes des fonctions de ce fichier
void loop(int);
void setupMaze();

int main(int argc, char** argv){

    ball = new Ball(0.5, 0.5, 0.05, 50);
    cameraStream = new CameraStream();
    namedWindow("aMAZEd Calibration");

    while(true){

        Mat currentFrame = cameraStream->getCurrentFrame();
        putText(currentFrame, "Press space bar to start calibration", Point2i(0, currentFrame.rows), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);

        imshow("aMAZEd Calibration", currentFrame);

        /// Si on appuie sur la touche espace
        if(waitKey(30) == 32) break;
    }

    Mat currentFrame = cameraStream->getCurrentFrame();
    double ratio = (double)currentFrame.cols / (double)currentFrame.rows;
    int width = 1000; /// Largeur de la fenêtre

    auto *glutMaster = new GlutMaster();
    window = new OpenGL(glutMaster, width, (int)(width / ratio), 0, 0, (char*)("aMAZEd"), ball, cameraStream);

    setupMaze();

    destroyWindow("aMAZEd Calibration");
    glutMaster->CallGlutMainLoop();

    delete cameraStream;
    delete window;
    delete angleModel;

    return 0;
}

void loop(int){

    EdgeDetection edgeDetection = EdgeDetection(cameraStream);

    vector<Point2i> coordCorner;
    Mat currentFrame = cameraStream->getCurrentFrame();
    coordCorner = edgeDetection.getCorner(currentFrame);
    edgeDetection.linesDetection(currentFrame, coordCorner);

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4) {
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        angleModel->setCurrentTransformation(&transformation);

//        ball->setX(angleModel->getAngleY() * 2 + 0.5);
//        ball->setY(-angleModel->getAngleX() * 2 + 0.5);

        ball->setAx(angleModel->getAngleY() / 5);
        ball->setAy(-angleModel->getAngleX() / 5);

        ball->updatePosition();

        cout << "VX=" << ball->getVx() << " VY=" << ball->getVy() << " VZ=" << ball->getVz() << endl;
//        cout << coordCorner << endl << endl;

        double p[16];
        double m[16];
        transformation.getProjectionMatrix(p);
        transformation.getModelviewMatrix(m);
        window->setProjectionMatrix(p);
        window->setModelviewMatrix(m);
    }


    glutPostRedisplay();

}

void setupMaze(){

    /// Calibration des couleurs
    EdgeDetection edgeDetection = EdgeDetection(cameraStream);
    edgeDetection.colorCalibration();

    Mat currentFrame = cameraStream->getCurrentFrame();

    vector<vector<Mat>> walls;
    vector<Point2i> coordCorner;
    vector<vector<Point2i>> lines;

    /// Tant que les 4 coins n'ont pas été détéctées
    do {
        currentFrame = cameraStream->getCurrentFrame();

        coordCorner = edgeDetection.getCorner(currentFrame);

        /// Detection des murs
        lines = edgeDetection.linesDetection(currentFrame, coordCorner);

    }while(coordCorner.size() != 4);


    Transformation *transformation = new Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);

    walls.clear();
    vector<Mat> wall;

    /// Pour chacune des lignes
    for (const auto &line : lines) {

        wall.clear();

        Mat pointImageA = Mat(3, 1, CV_64FC1);
        pointImageA.at<double>(0) = line[0].x;
        pointImageA.at<double>(1) = line[0].y;
        pointImageA.at<double>(2) = 1;

        Mat pointImageB = Mat(3, 1, CV_64FC1);
        pointImageB.at<double>(0) = line[1].x;
        pointImageB.at<double>(1) = line[1].y;
        pointImageB.at<double>(2) = 1;

        Mat homography = transformation->getHomography();

        /// Calcul des coordonées des murs sur le modele
        Mat pointModelA = homography * pointImageA;
        Mat pointModelB = homography * pointImageB;

        /// Normalisation des coordonnées
        for(int i = 0; i < 3; i++){
            pointModelA.at<double>(i) /= pointModelA.at<double>(2);
            pointModelB.at<double>(i) /= pointModelB.at<double>(2);
        }

        wall.push_back(pointModelA);
        wall.push_back(pointModelB);

        walls.push_back(wall);
    }

    /// Définition des murs
    wall.clear();
    Mat pointA = Mat(2, 1, CV_64FC1);
    Mat pointB = Mat(2, 1, CV_64FC1);
    Mat pointC = Mat(2, 1, CV_64FC1);
    Mat pointD = Mat(2, 1, CV_64FC1);

    pointA.at<double>(0) = 0.0;
    pointA.at<double>(1) = 0.0;
    pointB.at<double>(0) = 0.0;
    pointB.at<double>(1) = 1.0;
    pointC.at<double>(0) = 1.0;
    pointC.at<double>(1) = 1.0;
    pointD.at<double>(0) = 1.0;
    pointD.at<double>(1) = 0.0;

    wall.push_back(pointA);
    wall.push_back(pointB);
    walls.push_back(wall);
    wall.clear();

    wall.push_back(pointB);
    wall.push_back(pointC);
    walls.push_back(wall);
    wall.clear();

    wall.push_back(pointC);
    wall.push_back(pointD);
    walls.push_back(wall);
    wall.clear();

    wall.push_back(pointD);
    wall.push_back(pointA);
    walls.push_back(wall);
    wall.clear();

    window->setWalls(walls);

    angleModel = new AngleModel(transformation);

}