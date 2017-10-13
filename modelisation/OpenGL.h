#ifndef AMAZED_OPENGL_H
#define AMAZED_OPENGL_H

#include <GL/freeglut.h>
#include "../stream/CameraStream.h"
#include "GlutWindow.h"
#include "GlutMaster.h"

class OpenGL : public GlutWindow{

private:

    void drawAxes();
    void drawMazeGround();
    void drawBackground();
    void drawWalls();
    void loadTexture(GLuint id, cv::Mat img);

public:

    float const WALL_HEIGHT = -0.1f;

    /// Nombre d'images par seconde
    int const FPS = 30;

    /// Identifiants des textures
    GLuint const ID_TEXT_CAM = 1;
    GLuint const ID_TEXT_MAZE = 2;

    std::vector<std::vector<cv::Mat>> walls;

    void setWalls(const std::vector<std::vector<cv::Mat>> &walls);

    int height, width;
    int initPositionX, initPositionY;

    OpenGL(GlutMaster * glutMaster, int setWidth, int setHeight, int setInitPositionX, int setInitPositionY, char * title);
    ~OpenGL();

    void CallBackDisplayFunc() override;
    void CallBackReshapeFunc(int w, int h) override;
    void CallBackIdleFunc() override;

};


#endif //AMAZED_OPENGL_H
