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

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4) {
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 10);
        angleModel->setCurrentTransformation(&transformation);

        ball->setAx(angleModel->getAngleY() / 5);
        ball->setAy(-angleModel->getAngleX() / 5);

        ball->updatePosition();

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

    vector<Point2i> coordCorner;
    vector<Point2i> coordStartEnd;
    vector<vector<Point2i>> lines;

    /// Tant que les 4 coins n'ont pas été détéctées
    do {
        currentFrame = cameraStream->getCurrentFrame();
        do{
            coordStartEnd = edgeDetection.startEndDetection(currentFrame);
        }
        while( coordStartEnd.size() != 2);
        coordCorner = edgeDetection.getCorner(currentFrame);

        /// Detection des murs
        lines = edgeDetection.linesDetection(currentFrame, coordCorner);

    }while(coordCorner.size() != 4);


    Transformation *transformation = new Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 1, 10);


    /// Set la boule aux coordonnées du départ détecté
    Mat startPoint = Mat(3, 1, CV_64FC1);
    startPoint.at<double>(0) = coordStartEnd[0].x;
    startPoint.at<double>(1) = coordStartEnd[0].y;
    startPoint.at<double>(2) = 1;

    Mat homography = transformation->getHomography();

    /// Calcul des coordonées du départ sur le modele
    Mat pointModel = homography * startPoint;

    /// Normalisation des coordonnées
    for(int i = 0; i < 3; i++){
        pointModel.at<double>(i) /= pointModel.at<double>(2);
    }

    ///set la boule aux coordonnées du départ
    ball->setX(pointModel.at<double>(0));
    ball->setY(pointModel.at<double>(1));

    /// Calcul des coordonées des extrimités des murs
    vector<Wall> walls;
    for (const auto &line : lines) {

        Point2d pointImageA = transformation->getModelPointFromImagePoint(line[0]);
        Point2d pointImageB = transformation->getModelPointFromImagePoint(line[1]);

        Wall wall(pointImageA, pointImageB);

        walls.push_back(wall);
    }

    /// Murs extérieurs
    walls.emplace_back(Point2d(0, 0), Point2d(0, 1));
    walls.emplace_back(Point2d(1, 1), Point2d(0, 1));
    walls.emplace_back(Point2d(1, 1), Point2d(1, 0));
    walls.emplace_back(Point2d(1, 0), Point2d(0, 0));

    window->setWalls(walls);

    angleModel = new AngleModel(transformation);

}