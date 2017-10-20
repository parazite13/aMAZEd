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

void Ball::draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(this->x, this->y, this->z);
    glutSolidSphere(this->r, this->m, this->m);
    glPopMatrix();
}

void Ball::updatePosition() {
    vx += ax;
    vy += ay;

    x += vx;
    y += vy;

    if(x > 1 - r){
        x = 1 - r;
        vx = 0.0;
    }
    if(x < r){
        x = r;
        vx = 0.0;
    }

    if(y > 1 - r){
        y = 1 - r;
        vy = 0.0;
    }
    if(y < r){
        y = r;
        vy = 0.0;
    }
}
