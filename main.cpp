#include <opencv2/core/core.hpp>
#include "stream/CameraStream.h"
#include "analyse/EdgeDetection.h"
#include "modelisation/Transformation.h"
#include <GL/freeglut.h>
using namespace std;
using namespace cv;

//Textures
Mat textCam;
Mat textMaze;
GLuint const ID_TEXT_CAM = 1;
GLuint const ID_TEXT_MAZE = 2;

//Constantes
int const FPS = 30;
CameraStream cameraStream = CameraStream();

float h[16];
float m[16];
float p[16];

void loop(int){

    vector<Point2d> coordCorner;
    Mat currentFrame = cameraStream.getCurrentFrame();
    textCam = currentFrame;
    coordCorner = EdgeDetection::getCorner(currentFrame);
    EdgeDetection::linesDetection(currentFrame, coordCorner);

    if(coordCorner.size() == 4){
        Transformation transformation = Transformation(coordCorner, Size(currentFrame.cols, currentFrame.rows));
        transformation.getHomography(h);
        //transformation.getProjectionMatrix(p);
        //transformation.getModelviewMatrix(m);
    }

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
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

void drawBackground(){
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, -5.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, -5.0f);
    glEnd();
}

void drawBall(){
    glutSolidSphere(0.5f, 50, 50);
}

void loadTexture(GLuint id, Mat img) {
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            // Internal colour format to convert to
                 img.cols,      // Image width  i.e. 640 for Kinect in standard mode
                 img.rows,      // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 0x80E0,            // Valeur correspondante à BGR
                 GL_UNSIGNED_BYTE,  // Image data type
                 img.data);     // The actual image data itself

    glEnable(GL_TEXTURE_2D);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -10, 10);

    glPushMatrix();
    glMultMatrixf(h);
    //drawAxes();
    loadTexture(ID_TEXT_MAZE, textMaze);
    drawSquare();
    glPopMatrix();

    glPushMatrix();
    loadTexture(ID_TEXT_CAM, textCam);
    drawBackground();
    glPopMatrix();

    glutSwapBuffers();

    glutTimerFunc(1000 / FPS, loop, 0);
}

void resize(int width, int height){

    glViewport(0, 0, width, height);
}

int main(int argc, char** argv){

    int h = 0;
    int w = 0;
    CameraStream::getDesktopResolution(w, h);

    textMaze = imread("../assets/mazeGround.png"); //texture du sol du labyrinthe

    glutInit(&argc, argv);
    glutInitWindowSize(4*h/3, h);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("aMAZEd");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(resize);

    glutMainLoop();

    return 0;

}