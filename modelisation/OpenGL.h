#ifndef AMAZED_OPENGL_H
#define AMAZED_OPENGL_H

#include <GL/freeglut.h>
#include "../stream/CameraStream.h"
#include "GlutWindow.h"
#include "GlutMaster.h"

class OpenGL : public GlutWindow{

private:

    /// Hauteur des murs
    float const WALL_HEIGHT = -0.1f;

    /// Nombre d'images par seconde
    int const FPS = 30;

    /// Identifiants des textures
    GLuint const ID_TEXT_CAM = 1;
    GLuint const ID_TEXT_MAZE = 2;

    /// Matrice de projection
    double *p;

    /// Matrice de Modelview
    double *m;

    int height, width;
    int initPositionX, initPositionY;

    std::vector<std::vector<cv::Mat>> walls;

    CameraStream *cameraStream;
    cv::Mat textCam;
    cv::Mat textMaze;

    void drawAxes();
    void drawMazeGround();
    void drawBackground();
    void drawWalls();
    void loadTexture(GLuint id, cv::Mat img);

public:

    void setWalls(const std::vector<std::vector<cv::Mat>> &walls);
    void setProjectionMatrix(double* p);
    void setModelviewMatrix(double *m);

    OpenGL(GlutMaster * glutMaster, int setWidth, int setHeight, int setInitPositionX, int setInitPositionY, char * title, CameraStream * cameraStream);
    ~OpenGL() override;

    void CallBackDisplayFunc() override;
    void CallBackReshapeFunc(int w, int h) override;
    void CallBackIdleFunc() override;

};


#endif //AMAZED_OPENGL_H
