
#include <GL/glut.h>
#include <iostream>
#include "Hitbox.h"


Hitbox::Hitbox(cv::Point2d bot_left, cv::Point2d bot_right, cv::Point2d top_right, cv::Point2d top_left) {
    this->vertices[0] = bot_left;
    this->vertices[1] = bot_right;
    this->vertices[2] = top_right;
    this->vertices[3] = top_left;

    this->min = cv::Point2d(bot_left);
    this->max = cv::Point2d(top_right);
}

void Hitbox::drawSquare() {
    glColor3f(1.0,0,0);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    glVertex3d(this->vertices[0].x, this->vertices[0].y, 0.0f);
    glVertex3d(this->vertices[1].x, this->vertices[1].y, 0.0f);
    glVertex3d(this->vertices[2].x, this->vertices[2].y, 0.0f);
    glVertex3d(this->vertices[3].x, this->vertices[3].y, 0.0f);
    glEnd();
    glPopMatrix();
}
