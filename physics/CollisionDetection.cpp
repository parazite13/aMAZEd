
#include <GL/glut.h>
#include <iostream>
#include "CollisionDetection.h"

using namespace std;

bool CollisionDetection::intersect(Ball ball, Wall wall) {
    //Vector2d leftNormal = vect.rotate(-90degres)
    // i.e. v=[ax;ay] -> v_norm = [-ay:ax]
    double x1 = wall.getStart().x;
    double y1 = wall.getStart().y;
    double x2 = wall.getEnd().x;
    double y2 = wall.getEnd().y;
    Vector2d leftNormal(-(y2-y1), (x2-x1));
    Vector2d line(x1,y1,x2,y2);

    //calculating line's perpendicular distance to ball
    Vector2d c1_circle (ball.getX() - x1, ball.getY() - y1);
    double perpendicular = c1_circle.projectionOn(leftNormal);

    line.sortPoints();
    // True si collision, sinon false
    if( fabs(perpendicular) <= ball.getR() ) {
        return CollisionDetection::withinLine(ball, line) ;
    }

    return false;
}

bool CollisionDetection::withinLine(Ball ball, Vector2d vect) {
    double xmin = vect.get_x1()-ball.getR();
    double xmax = vect.get_x2()+ball.getR();
    double ymin = vect.get_y1();
    double ymax = vect.get_y2();

    if(ymin > ymax) {
        double tmp = ymin;
        ymin = ymax;
        ymax = tmp;
    }

    ymin -= ball.getR();
    ymax += ball.getR();

    if(vect.get_x1() != vect.get_x2()) {

        /// si la position x est entre x1 et x2
        if(ball.getX() >= xmin && ball.getX() <= xmax) {
            /// Dans le cas ou y1 != y2
            if(vect.get_y1() != vect.get_y2()) {

                /// si la position y est entre y1 et y2
                if(ball.getY() >= ymin && ball.getY() <= ymax)
                    return true;
            }   /// Cas ou y1 = y2
            else {
                return true;
            }
        }
    }
        /// x1 = x2, forcement y1 != y2
    else {
        if(ball.getY() >= ymin && ball.getY() <= ymax)
            return true;
    }
    return false;
}

bool CollisionDetection::findCollisions(Ball ball, vector<Wall> walls, vector<Wall> &touchedWalls) {
    bool result = false;
    for(Wall current: walls) {
        if(intersect(ball, current)) {
            touchedWalls.push_back(current);
            result = true;
        }
    }
    return result;
}

/////////////////////////////////////       TESTS         //////////////////////////////////////////////////

bool CollisionDetection:: hasArrived(Ball *ball, cv::Point2d *point){
    return ball->getX() >= point->x - ball->getR()
           && ball->getX() <= point->x + ball->getR()
           && ball->getY() >= point->y - ball->getR()
           && ball->getY() <= point->y + ball->getR();
}

bool CollisionDetection::intersect(Circle circle, Vector2d vect) {
    //Vector2d leftNormal = vect.rotate(-90degres)
    // i.e. v=[ax;ay] -> v_norm = [-ay:ax]
    double x1 = vect.get_x1();
    double y1 = vect.get_y1();
    double x2 = vect.get_x2();
    double y2 = vect.get_y2();
    Vector2d leftNormal(-(y2-y1), (x2-x1));
    Vector2d line(x1,y1,x2,y2);

    //calculating line's perpendicular distance to ball
    Vector2d c1_circle (circle.center.x - x1, circle.center.y - y1);
    double perpendicular = c1_circle.projectionOn(leftNormal);

    line.sortPoints();
    // True si collision, sinon false
    if( fabs(perpendicular) <= circle.radius ) {
        return CollisionDetection::withinLine(circle, line) ;
    }

    return false;
}

bool CollisionDetection::withinLine(Circle circle, Vector2d vect) {
    /*cout<<"x1debut "<<vect.get_x1();
    cout<<" ,x2debut "<<vect.get_x2();
    cout<<" ,y1debut "<<vect.get_y1();
    cout<<" ,y2debut "<<vect.get_y2()<<endl;*/

    double xmin = vect.get_x1()-circle.radius;
    double xmax = vect.get_x2()+circle.radius;
    double ymin = vect.get_y1();
    double ymax = vect.get_y2();

    if(ymin > ymax) {
        double tmp = ymin;
        ymin = ymax;
        ymax = tmp;
    }

    ymin -= circle.radius;
    ymax += circle.radius;

    /*cout<<"circle x : "<<circle.center.x;
    cout<<"// xmin : "<<xmin<<", ymin : "<<ymin<<endl;
    cout<<"circle y : "<<circle.center.y;
    cout<<"// xmax : "<<xmax<<", ymax : "<<ymax<<endl;*/

    if(vect.get_x1() != vect.get_x2()) {

        /// si la position x est entre x1 et x2
        if(circle.center.x >= xmin && circle.center.x <= xmax) {
            /// Dans le cas ou y1 != y2
            if(vect.get_y1() != vect.get_y2()) {

                /// si la position y est entre y1 et y2
                if(circle.center.y >= ymin && circle.center.y <= ymax)
                    return true;
            }   /// Cas ou y1 = y2
            else {
                return true;
            }
        }
    }
        /// x1 = x2, forcement y1 != y2
    else {
        if(circle.center.y >= ymin && circle.center.y <= ymax)
            return true;
    }
    return false;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool CollisionDetection::onSegment(cv::Point2d p, cv::Point2d q, cv::Point2d r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int CollisionDetection::orientation(cv::Point2d p, cv::Point2d q, cv::Point2d r) {
    // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    double val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    // incertitude
    double epsilon = 10^-10;
    if ( fabs(val+epsilon) < fabs(0+epsilon)) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
//                                                p1                        q1                   p2&q2
//bool CollisionDetection::doIntersect(cv::Point2d circleCenter, cv::Point2d nextPosition, Hitbox box) {
//
//    vector<cv::Point2d> nearestEdge = CollisionDetection::selectNearestEdge(circleCenter, box);
//    cv::Point2d p2 = nearestEdge[0];
//    cv::Point2d q2 = nearestEdge[1];
//
//    // Find the four orientations needed for general and
//    // special cases
//    int o1 = orientation(circleCenter, nextPosition, p2);
//    int o2 = orientation(circleCenter, nextPosition, q2);
//    int o3 = orientation(p2, q2, circleCenter);
//    int o4 = orientation(p2, q2, nextPosition);
//
//    // General case
//    if (o1 != o2 && o3 != o4)
//        return true;
//
//    // Special Cases
//    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
//    if (o1 == 0 && CollisionDetection::onSegment(circleCenter, p2, nextPosition)) return true;
//
//    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
//    if (o2 == 0 && CollisionDetection::onSegment(circleCenter, q2, nextPosition)) return true;
//
//    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
//    if (o3 == 0 && CollisionDetection::onSegment(p2, circleCenter, q2)) return true;
//
//    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
//    if (o4 == 0 && CollisionDetection::onSegment(p2, nextPosition, q2)) return true;
//
//    return false; // Doesn't fall in any of the above cases
//}
//
//vector<cv::Point2d> CollisionDetection::selectNearestEdge(cv::Point2d circleCenter, Hitbox box) {
//    vector<cv::Point2d> nearestEdge;
//    cv::Point2d min1, min2;
//
//    // A(xa, ya) et B(xb, yb) -> distance AB² = (xb-xa)² + (yb-ya)²
//    double distance;
//    double distanceMin = 99999999;
//    for(cv::Point2d point : box.vertices) {
//        distance = (point.x - circleCenter.x)*(point.x - circleCenter.x) + (point.y -circleCenter.y)*(point.x - circleCenter.x);
//        if(distance < distanceMin) {
//            min1 = point;
//            distanceMin = distance;
//        }
//    }
//
//    distanceMin = 99999999;
//    for(cv::Point2d point : box.vertices) {
//        distance = (point.x - circleCenter.x) + (point.y -circleCenter.y);
//        if(distance < distanceMin && point != min1) {
//            min2 = point;
//            distanceMin = distance;
//        }
//    }
//
//    nearestEdge.push_back(min1);
//    nearestEdge.push_back(min2);
//
//    return nearestEdge;
//}

/// Test !
/// Separer le dessin de la creation sinon lors de l'affichage, les points reviennent a leur position d'origine dans l'objet lors d'un deplacement
CollisionDetection::Circle CollisionDetection::createCircle(float radius, cv::Point2d center) {
    CollisionDetection::Circle circle = CollisionDetection::Circle();
    circle.radius = radius;
    circle.velocity.push_back(0) ;
    circle.velocity.push_back(0) ;
    circle.center.x = center.x;
    circle.center.y = center.y;

    return circle;
}

/// Test !
void CollisionDetection::drawCircle(CollisionDetection::Circle circle){
    float x, y;
    int h = 0;
    int k = 0;

    glColor3f(1.0,0,0);
    glPushMatrix();
    glBegin(GL_LINES);
    for (int i = 0; i < 180; i++)
    {
        x =  circle.radius* cos(i) - h;
        y = circle.radius * sin(i) + k;
        glVertex3f(x + k,y - h,0);

        x = circle.radius * cos(i + 0.1) - h;
        y = circle.radius * sin(i + 0.1) + k;
        glVertex3f(x + k,y - h,0);
    }
    glEnd();
    glPopMatrix();
}


