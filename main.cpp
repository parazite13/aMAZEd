#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "stream/CameraStream.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"
#include <GL/freeglut.h>

using namespace std;
using namespace cv;

CameraStream cameraStream = CameraStream();
float h[16];

void loop(int){

    vector<Point2d> coordCorner;
    Mat currentFrame = cameraStream.getCurrentFrame();
    EdgeDetection::linesDetection(currentFrame, 220);
    coordCorner = EdgeDetection::getCorner(currentFrame);

    if(coordCorner.size() == 4){
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows));
        transformation.getHomography(h);
    }

    //imshow("Camera", currentFrame);

    glutPostRedisplay();

}

void drawAxes(){

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

void drawSquare(){

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2d(0, 1); glVertex3f(0.0f, 0.0f, 0.0);
    glTexCoord2d(0, 0); glVertex3f(0.0f, 1.0f, 0.0);
    glTexCoord2d(1, 0); glVertex3f(1.0f, 1.0f, 0.0);
    glTexCoord2d(1, 1); glVertex3f(1.0f, 0.0f, 0.0);
    glEnd();

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -10, 10);
    glMultMatrixf(h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawAxes();
    drawSquare();

    glutSwapBuffers();

    glutTimerFunc(30, loop, 0);
}

void resize(int width, int height){

    glViewport(0, 0, width, height);

}


int main(int argc, char** argv){

    int h = 0;
    int w = 0;
    CameraStream::getDesktopResolution(w, h);
    //namedWindow("Camera", WINDOW_NORMAL);
    //resizeWindow("Camera", 4*h/3, h);

    glutInit(&argc, argv);

    glutInitWindowSize(w, h);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("aMAZEd");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(resize);

    glutMainLoop();

    return 0;

}