#include "modelisation/OpenGL.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"
#include "physics/AngleModel.h"
#include "physics/CollisionDetection.h"

using namespace cv;
using namespace std;

CameraStream *cameraStream = nullptr;
OpenGL *window = nullptr;
AngleModel *angleModel = nullptr;
Ball *ball = nullptr;

/// Pour afficher les FPS
int frame=0,time,timebase=0;
double fps = 0.0;

/// Prototypes des fonctions de ce fichier
void loop(int);
void setupMaze();

int main(int argc, char** argv){

    ball = new Ball(0.5, 0.5, 0.03, 50);
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

    /// Affichage FPS
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
    }
    window->setFps(fps);

    EdgeDetection edgeDetection = EdgeDetection();

    vector<Point2i> coordCorner;
    Mat currentFrame = cameraStream->getCurrentFrame();
    coordCorner = edgeDetection.getCorner(currentFrame);

    /// Si les 4 coins ont été détéctées
    if(coordCorner.size() == 4 && !edgeDetection.isReversed(coordCorner)) {
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 0.1, 20);
        angleModel->setCurrentTransformation(&transformation);

        vector<Wall> walls;
        if(CollisionDetection::findCollisions(ball, window->getWalls(), walls)){

            bool verticalCollision = false;
            bool horizontalCollision = false;

            for(auto &wall : walls){
                if(!verticalCollision && wall.isVertical()) verticalCollision = true;
                if(!horizontalCollision && !wall.isVertical()) horizontalCollision = true;
            }

            if(verticalCollision){
                ball->setX(ball->getX() - ball->getVx());
                if(ball->getVx() > 0){
                    ball->setVx(-0.005);
                }else{
                    ball->setVx(0.005);
                }
                ball->setAx(0);
            }

            if(horizontalCollision){
                ball->setY(ball->getY() - ball->getVy());
                if(ball->getVy() > 0){
                    ball->setVy(-0.005);
                }else{
                    ball->setVy(0.005);
                }
                ball->setAy(0);
            }

        }else{
            ball->setAx(angleModel->getAngleY() / 10);
            ball->setAy(-angleModel->getAngleX() / 10);
        }


        CollisionDetection::findCollisions(ball, window->getWalls(), walls);

        ball->updatePosition();

        if(CollisionDetection::hasArrived(ball, window->getEndPoint())){
            glutLeaveMainLoop();
            destroyAllWindows();
            namedWindow("Congratulations");
            Mat frame = Mat(200, 380, CV_8UC3, Scalar(0,0,0));
            putText(frame, "You are aMAZEing !", Point2i(10, 100), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 2);
            imshow("Congratulations", frame);
            waitKey(0);
            return;
        }

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
    EdgeDetection edgeDetection = EdgeDetection();

    Mat currentFrame = cameraStream->getCurrentFrame();

    vector<Point2i> coordCorner;
    vector<Point2i> coordStartEnd;
    vector<vector<Point2i>> lines;

    /// Tant que les 4 coins n'ont pas été détéctées
    do {
        currentFrame = cameraStream->getCurrentFrame();
        coordStartEnd = edgeDetection.startEndDetection(currentFrame);
        coordCorner = edgeDetection.getCorner(currentFrame);

        /// Detection des murs
        lines = edgeDetection.linesDetection(currentFrame, coordCorner);

    }while(coordCorner.size() != 4 &&  coordStartEnd.size() != 2);

    Transformation *transformation = new Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows), 1, 10);

    ///point d'arrivée sauvegarde
    Point2d *pointModelEnd = new Point2d(transformation->getModelPointFromImagePoint(coordStartEnd[1]));
    window->setEndPoint(pointModelEnd);

    ///set la boule aux coordonnées du départ détectés
    cv::Point2d pointModelStart = transformation->getModelPointFromImagePoint(coordStartEnd[0]);

    ///set la boule aux coordonnées du départ
    ball->setX(pointModelStart.x);
    ball->setY(pointModelStart.y);

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