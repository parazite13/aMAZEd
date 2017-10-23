#include "Ball.h"

Ball::Ball(double x, double y, double r, GLint m) {
    this->x = x;
    this->y = y;
    this->z = -r;
    this->r = r;
    this->m = m;
    this->vx = 0;
    this->vy = 0;
    this->vz = 0;
    this->ax = 0;
    this->ay = 0;
    this->az = 0;
}

double Ball::getX() const {
    return x;
}

void Ball::setX(double x) {
    Ball::x = x;
}

double Ball::getY() const {
    return y;
}

void Ball::setY(double y) {
    Ball::y = y;
}

double Ball::getZ() const {
    return z;
}

void Ball::setZ(double z) {
    Ball::z = z;
}

double Ball::getR() const {
    return r;
}

double Ball::getAx() const {
    return ax;
}
void Ball::setAx(double a) {
    Ball::ax = a;
}

double Ball::getAy() const {
    return ay;
}
void Ball::setAy(double a) {
    Ball::ay = a;
}

double Ball::getAz() const {
    return az;
}
void Ball::setAz(double a) {
    Ball::az = a;
}

double Ball::getVx() const {
    return vx;
}

void Ball::setVx(double vx) {
    Ball::vx = vx;
}

double Ball::getVy() const {
    return vy;
}

void Ball::setVy(double vy) {
    Ball::vy = vy;
}

double Ball::getVz() const {
    return vz;
}

void Ball::setVz(double vz) {
    Ball::vz = vz;
}

GLint Ball::getM() const {
    return m;
}

void Ball::draw(bool shadow){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(this->x, this->y, this->z);
    if(shadow){
        glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
        glutSolidSphere(this->r, this->m, this->m);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else{
        glutSolidSphere(this->r, this->m, this->m);
    }
    glPopMatrix();
}

void Ball::updatePosition() {
    vx += ax;
    vy += ay;

    x += vx;
    y += vy;

    if(x > 1 - r && vx > 0){
        x = 1 - r;
        vx = 0.0;
    }
    if(x < r && vx < 0){
        x = r;
        vx = 0.0;
    }

    if(y > 1 - r && vy > 0){
        y = 1 - r;
        vy = 0.0;
    }
    if(y < r && vy < 0){
        y = r;
        vy = 0.0;
    }
}
