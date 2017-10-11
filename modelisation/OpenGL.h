#ifndef AMAZED_OPENGL_H
#define AMAZED_OPENGL_H

#include <GL/freeglut.h>
#include "../stream/CameraStream.h"

class OpenGL {

public:

    /// Nombre d'images par seconde
    static int const FPS = 30;

    /// Identifiants des textures
    static GLuint const ID_TEXT_CAM = 1;
    static GLuint const ID_TEXT_MAZE = 2;

    static void init(int argc, char** argv);

private:

    static void drawAxes();
    static void drawMazeGround();
    static void drawBackground();
    static void loadTexture(GLuint id, cv::Mat img);
    static void display();
    static void resize(int width, int height);
};


#endif //AMAZED_OPENGL_H
