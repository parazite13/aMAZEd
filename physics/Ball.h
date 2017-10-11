#ifndef AMAZED_BALL_H
#define AMAZED_BALL_H

#include <GL/freeglut.h>

class Ball  {
private:
    float x;    //position selon l'axe x
    float y;    //position selon l'axe y
    float z;    //position selon l'axe z
    float r;    //rayon de la boule
    float ax;   //acceleration selon l'axe x
    float ay;   //acceleration selon l'axe y
    float az;   //acceleration selon l'axe z
    float vx;   //vitesse selon l'axe x
    float vy;   //vitesse selon l'axe y
    float vz;   //vitesse selon l'axe z
    GLint m;    //maillage

public:
    Ball(float x, float y, float z, float r, GLint m);

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getZ() const;

    void setZ(float z);

    float getR() const;

    float getAx() const;

    void setAx(float a);

    float getAy() const;

    void setAy(float a);

    float getAz() const;

    void setAz(float a);

    float getVx() const;

    void setVx(float vx);

    float getVy() const;

    void setVy(float vy);

    float getVz() const;

    void setVz(float vz);

    GLint getM() const;

    void draw();

    void updatePosition();
};

#endif //AMAZED_BALL_H
