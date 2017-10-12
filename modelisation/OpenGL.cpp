#include <GL/freeglut.h>
#include <opencv2/core/core.hpp>
#include "OpenGL.h"

using namespace cv;
using namespace std;

extern Mat textMaze;
extern Mat textCam;
extern double p[16];
extern double m[16];

void loop(int);

OpenGL::OpenGL(GlutMaster * glutMaster, int setWidth, int setHeight, int setInitPositionX, int setInitPositionY, char * title){

    this->width  = setWidth;
    this->height = setHeight;

    this->initPositionX = setInitPositionX;
    this->initPositionY = setInitPositionY;

    this->homography = Mat(3, 3, CV_64FC1);

    textMaze = imread("../assets/mazeGround.png"); //texture du sol du labyrinthe

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(initPositionX, initPositionY);
    glViewport(0, 0, width, height);   // This may have to be moved to after the next line on some platforms

    glutMaster->CallGlutCreateWindow(title, this);

    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 	// Nécessaire pour éviter une déformation de l'image
}

OpenGL::~OpenGL(){
    glutDestroyWindow(windowID);
}

void OpenGL::CallBackDisplayFunc(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawBackground();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(p);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(m);
//    drawAxes();
    drawMazeGround();

    drawWalls();

    glutSwapBuffers();

    glutTimerFunc((unsigned int)1000 / FPS, loop, 0);
}

void OpenGL::CallBackReshapeFunc(int w, int h){

    width = w;
    height= h;

    glViewport(0, 0, width, height);
    CallBackDisplayFunc();
}

void OpenGL::CallBackIdleFunc(){
    CallBackDisplayFunc();
}

void OpenGL::loadTexture(GLuint id, Mat img) {

    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            // Internal colour format to convert to
                 img.cols,          // Image width  i.e. 640 for Kinect in standard mode
                 img.rows,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 0x80E0,            // Valeur correspondante à GL_BGR
                 GL_UNSIGNED_BYTE,  // Image data type
                 img.data);         // The actual image data itself

    glEnable(GL_TEXTURE_2D);
}

void OpenGL::drawAxes(){

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.25f, 0.0f);
    glVertex3f(0.75f, -0.25f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.0f, 0.25f);
    glVertex3f(0.75f, 0.0f, -0.25f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.75f, 0.25f);
    glVertex3f(0.0f, 0.75f, -0.25f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.25f, 0.75f, 0.0f);
    glVertex3f(-0.25f, 0.75f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.25f, 0.0f, 0.75f);
    glVertex3f(-0.25f, 0.0f, 0.75f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.25f, 0.75f);
    glVertex3f(0.0f, -0.25f, 0.75f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    glColor4ub(255, 255, 0, 255);
    glRasterPos3f(1.1f, 0.0f, 0.0f);

    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
    glRasterPos3f(0.0f, 1.1f, 0.0f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
    glRasterPos3f(0.0f, 0.0f, 1.1f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
}

void OpenGL::drawMazeGround(){
    loadTexture(ID_TEXT_MAZE, textMaze);
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

void OpenGL::drawBackground() {
    loadTexture(ID_TEXT_MAZE, textCam);
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0, 0.0f, -5.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, -5.0f);
    glEnd();
}

void OpenGL::drawWalls() {

    glColor3f(1, 0, 0);

    /// Pour chaucune des lignes
    for(const auto &line : lines){

        Mat pointImageA = Mat(3, 1, CV_64FC1);
        pointImageA.at<double>(0) = line[0].x;
        pointImageA.at<double>(1) = line[0].y;
        pointImageA.at<double>(2) = 0;

        Mat pointImageB = Mat(3, 1, CV_64FC1);
        pointImageB.at<double>(0) = line[1].x;
        pointImageB.at<double>(1) = line[1].y;
        pointImageB.at<double>(2) = 0;

        Mat pointModelA = homography * pointImageA;
        Mat pointModelB = homography * pointImageB;

        glBegin(GL_POLYGON);

        glVertex3d(pointModelA.at<double>(0), pointModelA.at<double>(1), 0);
        glVertex3d(pointModelA.at<double>(0), pointModelA.at<double>(1), WALL_HEIGHT);
        glVertex3d(pointModelB.at<double>(0), pointModelB.at<double>(1), WALL_HEIGHT);
        glVertex3d(pointModelB.at<double>(0), pointModelB.at<double>(1), 0);

        /*cout << "LES POINTS" << endl;
        cout << pointModelA.at<double>(0) << ", " << pointModelA.at<double>(1) << ", " << 0 << endl;
        cout << pointModelA.at<double>(0) << ", " << pointModelA.at<double>(1) << ", " << WALL_HEIGHT << endl;
        cout << pointModelB.at<double>(0) << ", " << pointModelB.at<double>(1) << ", " << WALL_HEIGHT << endl;
        cout << pointModelB.at<double>(0) << ", " << pointModelB.at<double>(1) << ", " << 0 << endl;*/

        glEnd();

        cout << line << endl;

    }

    glColor3f(1.0, 1.0, 1.0);

}

void OpenGL::setHomography(cv::Mat &mat) {
    this->homography = mat;
}

void OpenGL::setLines(std::vector<std::vector<cv::Point2f>> &lines) {
    this->lines = lines;
}
