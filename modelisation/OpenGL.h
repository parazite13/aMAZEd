#ifndef AMAZED_OPENGL_H
#define AMAZED_OPENGL_H

#include <GL/freeglut.h>
#include "../stream/CameraStream.h"
#include "GlutWindow.h"
#include "GlutMaster.h"
#include "../physics/Ball.h"
#include "../physics/Wall.h"

class OpenGL : public GlutWindow{

private:

    /// Nombre d'images par seconde
    int const MAX_FPS = 100;

    /// Index des textures
    int const ID_TEXT_CAM = 1;
    int const ID_TEXT_MAZE = 2;
    int const ID_TEXT_WALL = 3;
    GLuint textArray[3];

    /// Matrice de projection
    double *p;

    /// Matrice de Modelview
    double *m;

    int height, width;
    int initPositionX, initPositionY;

    std::vector<Wall> walls;

    Ball *ball;
    CameraStream *cameraStream;
    cv::Mat textCam;
    cv::Mat textMaze;
    cv::Mat textWall;

    void drawAxes();
    void drawMazeGround();
    void drawBackground();
    void drawWalls();
    void loadTexture(GLuint id, cv::Mat img);

    GLfloat sol[3][3] = {{0.0f,0.0f,0.0f},
                         {1.0f,0.0f,0.0f},
                         {0.0f,1.0f,0.0f}};
    GLfloat light_pos[4] = {0.0f, 0.0f, 10.0f, 1.0};
    GLfloat ombre[4][4];


public:

    void setWalls(const std::vector<Wall> &walls);
    void setProjectionMatrix(const double* p);
    void setModelviewMatrix(const double *m);
    void applicateMaterial();
    void applicateLight();
    void shadowMatrix(GLfloat points_plan[3][3], const GLfloat lightPos[4], GLfloat destMat[4][4]);
    void vecteurUnite(float *vector);
    void normal(float v[3][3], float out[3]);

    OpenGL(GlutMaster * glutMaster, int setWidth, int setHeight, int setInitPositionX, int setInitPositionY, char * title, Ball *ball, CameraStream * cameraStream);

    ~OpenGL() override;
    void CallBackDisplayFunc() override;
    void CallBackReshapeFunc(int w, int h) override;


    void CallBackIdleFunc() override;
};


#endif //AMAZED_OPENGL_H
