#include <GL/freeglut.h>
#include <opencv2/core/core.hpp>
#include "OpenGL.h"

using namespace cv;
using namespace std;

extern Mat textMaze;
extern float p[16];
extern float m[16];

float mat[16];   // DEBUG

/// Fonction qui s'appelle en boucle définie dans le main
void loop(int);

void OpenGL::init(int argc, char** argv) {

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

void OpenGL::drawSquare(){
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

void OpenGL::drawBackground(){
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, -5.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, -5.0f);
    glEnd();
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

void OpenGL::display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(0, 1, 0, 1, -10, 10);
    glMultMatrixf(p);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glMultMatrixf(h);
    glMultMatrixf(m);

    drawAxes();
    loadTexture(ID_TEXT_MAZE, textMaze);
    drawSquare();

//    loadTexture(ID_TEXT_CAM, textCam);
//    drawBackground();

    cout << "OPENGL PROJ" << endl;
    glGetFloatv(GL_PROJECTION_MATRIX, mat);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << mat[i + j * 4] << ", ";
        }
        cout << endl;
    }

    cout << "OPENGL MODELVIEW" << endl;
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << mat[i + j * 4] << ", ";
        }
        cout << endl;
    }

    glutSwapBuffers();

    glutTimerFunc(1000 / FPS, loop, 0);
}

void OpenGL::resize(int width, int height){
    glViewport(0, 0, width, height);
}