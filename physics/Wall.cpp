#include <GL/gl.h>
#include "Wall.h"

using namespace cv;

Wall::Wall() = default;

void Wall::draw() {

    glBegin(GL_POLYGON);
    glTexCoord2d(0, 0); glVertex3d(start.x, start.y, 0);
    glTexCoord2d(1, 0); glVertex3d(end.x, end.y, 0);
    glTexCoord2d(1, 0.2); glVertex3d(end.x, end.y, WALL_HEIGHT);
    glTexCoord2d(0, 0.2); glVertex3d(start.x, start.y, WALL_HEIGHT);
    glEnd();

}

Wall::Wall(cv::Point2d start, cv::Point2d end) {
    this->start = start;
    this->end = end;
}

const Point2d &Wall::getStart() const {
    return start;
}

const Point2d &Wall::getEnd() const {
    return end;
}

void Wall::setStart(const Point2d &start) {
    Wall::start = start;
}

void Wall::setEnd(const Point2d &end) {
    Wall::end = end;
}

Wall& Wall::operator=(const Wall& wall) {
    this->start = wall.getStart();
    this->end = wall.getEnd();
}
