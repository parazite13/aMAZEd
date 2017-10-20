#include <GL/freeglut.h>
#include <opencv2/core/core.hpp>
#include "OpenGL.h"

using namespace cv;
using namespace std;

/// Fonction appelé en boucle et définie dans le main
void loop(int);
OpenGL::OpenGL(GlutMaster * glutMaster, int setWidth, int setHeight, int setInitPositionX, int setInitPositionY, char * title, Ball *ball, CameraStream * cameraStream){
    this->ball = ball;
    this->cameraStream = cameraStream;

    this->width  = setWidth;
    this->height = setHeight;

    this->initPositionX = setInitPositionX;
    this->initPositionY = setInitPositionY;

    this->p = new double[16];
    this->m = new double[16];

    this->textMaze = imread("../assets/mazeGround.png"); //texture du sol du labyrinthe
    this->textWall = imread("../assets/mazeWall.png"); //texture du mur du labyrinthe

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(this->width, this->height);
    glutInitWindowPosition(this->initPositionX, this->initPositionY);
    glViewport(0, 0, this->width, this->height);

    glutMaster->CallGlutCreateWindow(title, this);
    applicateLight();
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 	// Nécessaire pour éviter une déformation de l'image
}

OpenGL::~OpenGL(){
    glutDestroyWindow(windowID);
    delete p;
    delete m;
}

void OpenGL::CallBackDisplayFunc(){

    drawAxes();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    this->textCam = cameraStream->getCurrentFrame();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawBackground();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(this->p);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(this->m);
//    drawAxes();
    drawMazeGround();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    drawWalls();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    applicateMaterial();
    ball->draw();
    glPopMatrix();

    glutSwapBuffers();

    glutTimerFunc((unsigned int)1000 / FPS, loop, 0);
}

void OpenGL::CallBackReshapeFunc(int w, int h){

    this->width = w;
    this->height= h;

    glViewport(0, 0, this->width, this->height);
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

    glTexImage2D(GL_TEXTURE_2D,     /// Type of texture
                 0,                 /// Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            /// Internal colour format to convert to
                 img.cols,          /// Image width  i.e. 640 for Kinect in standard mode
                 img.rows,          /// Image height i.e. 480 for Kinect in standard mode
                 0,                 /// Border width in pixels (can either be 1 or 0)
                 0x80E0,            /// Valeur correspondante à GL_BGR
                 GL_UNSIGNED_BYTE,  /// Image data type
                 img.data);         /// The actual image data itself

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

    glColor3f(1, 1, 1);
}

void OpenGL::drawMazeGround(){
    loadTexture(ID_TEXT_MAZE, this->textMaze);

    /// Plateau de jeu
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

void OpenGL::drawBackground() {
    loadTexture(ID_TEXT_CAM, this->textCam);
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);glVertex3f(0.0, 0.0f, -5.0f);
    glTexCoord2d(0, 0);glVertex3f(0.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 0);glVertex3f(1.0f, 1.0f, -5.0f);
    glTexCoord2d(1, 1);glVertex3f(1.0f, 0.0f, -5.0f);
    glEnd();
}

void OpenGL::applicateMaterial() {
    GLfloat Lemission[4] = {0.023125f, 0.023125f, 0.023125f, 1.0f};
    GLfloat Ldiffuse[4] = {0.2775f, 0.2775f, 0.2775f, 1.0f};
    GLfloat Lspecular[4] = {0.773911f, 0.773911f, 0.773911f, 1.0f};
    GLfloat Lshininess[1] = {89.6f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lemission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Ldiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Lspecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Lshininess);
}

void OpenGL::applicateLight() {
    //Light
    glEnable(GL_LIGHTING);
    GLfloat LPosition[4] =  { 1.5f, 1.5f, 0.0f, 1.0};
    GLfloat LAmbient[4] =  { 1.4, 1.4, 1.4, 1.0};
    GLfloat LDiffuse[4] =  {2.0, 2.0, 2.0, 1.0};
    GLfloat LSpecular[4] =  {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, LPosition);   // position
    glLightfv(GL_LIGHT0, GL_AMBIENT, LAmbient );    // couleur de la forme
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LDiffuse);     // couleur de la lumière
    glLightfv(GL_LIGHT0, GL_SPECULAR, LSpecular);   // couleur du reflet
    glEnable(GL_LIGHT0);
}

void OpenGL::drawWalls() {

    glPushMatrix();
    loadTexture(ID_TEXT_WALL, this->textWall);

//    glEnable(GL_LIGHTING);
//    GLfloat LPosition[4] =  { 0.0f, 1.0, -3.0f, 1.0};
//    GLfloat LAmbient[4] =  { 10.4, 10.4, 10.4, 1.0};
//    GLfloat LDiffuse[4] =  {2.0, 2.0, 2.0, 1.0};
//    GLfloat LSpecular[4] =  {1.0, 1.0, 1.0, 1.0};
//    glLightfv(GL_LIGHT0, GL_POSITION, LPosition);   // position
//    glLightfv(GL_LIGHT0, GL_AMBIENT, LAmbient );    // couleur de la forme
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, LDiffuse);     // couleur de la lumière
//    glLightfv(GL_LIGHT0, GL_SPECULAR, LSpecular);   // couleur du reflet
//    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    /// Pour chacune des lignes
    for(auto &wall : this->walls){

        Mat pointModelA = wall[0];
        Mat pointModelB = wall[1];

        glBegin(GL_POLYGON);

        glTexCoord2d(0, 0); glVertex3d(pointModelA.at<double>(0), pointModelA.at<double>(1), 0);
        glTexCoord2d(0, 0.3); glVertex3d(pointModelA.at<double>(0), pointModelA.at<double>(1), WALL_HEIGHT);
        glTexCoord2d(1, 0.3); glVertex3d(pointModelB.at<double>(0), pointModelB.at<double>(1), WALL_HEIGHT);
        glTexCoord2d(1, 0); glVertex3d(pointModelB.at<double>(0), pointModelB.at<double>(1), 0);

        glEnd();

    }
    glPopMatrix();
}

void OpenGL::setWalls(const vector<vector<Mat>> &walls) {
    this->walls = walls;
}

void OpenGL::setProjectionMatrix(double *p) {
    for(int i = 0; i < 16; i++){
        this->p[i] = p[i];
    }
}

void OpenGL::setModelviewMatrix(double *m) {
    for(int i = 0; i < 16; i++){
        this->m[i] = m[i];
    }
}