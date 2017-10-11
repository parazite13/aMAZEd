#include "Ball.h"

Ball::Ball(float x, float y, float z, float r, GLint m) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->m = m;
    this->vx = 0;
    this->vy = 0;
    this->vz = 0;
    this->ax = 0;
    this->ay = 0;
    this->az = 0;
}

float Ball::getX() const {
    return x;
}

void Ball::setX(float x) {
    Ball::x = x;
}

float Ball::getY() const {
    return y;
}

void Ball::setY(float y) {
    Ball::y = y;
}

float Ball::getZ() const {
    return z;
}

void Ball::setZ(float z) {
    Ball::z = z;
}

float Ball::getR() const {
    return r;
}

float Ball::getAx() const {
    return ax;
}
void Ball::setAx(float a) {
    Ball::ax = a;
}

float Ball::getAy() const {
    return ay;
}
void Ball::setAy(float a) {
    Ball::ay = a;
}

float Ball::getAz() const {
    return az;
}
void Ball::setAz(float a) {
    Ball::az = a;
}

float Ball::getVx() const {
    return vx;
}

void Ball::setVx(float vx) {
    Ball::vx = vx;
}

float Ball::getVy() const {
    return vy;
}

void Ball::setVy(float vy) {
    Ball::vy = vy;
}

float Ball::getVz() const {
    return vz;
}

void Ball::setVz(float vz) {
    Ball::vz = vz;
}

GLint Ball::getM() const {
    return m;
}

void Ball::draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glutSolidSphere(this->r, this->m, this->m);
    glPopMatrix();
}

void Ball::updatePosition() {
    x += vx;
    y += vy;
    z += vz;
}
