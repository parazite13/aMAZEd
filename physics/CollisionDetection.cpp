
#include <GL/glut.h>
#include <iostream>
#include "CollisionDetection.h"

using namespace std;

bool CollisionDetection::intersect(Ball *ball, Wall wall) {
    /// Calcul du vecteur leftNormal par rapport au mur
    /// --> v=[ax;ay] -> v_leftNorm = [-ay;ax]
    double x1 = wall.getStart().x;
    double y1 = wall.getStart().y;
    double x2 = wall.getEnd().x;
    double y2 = wall.getEnd().y;
    Vector2d leftNormal(-(y2-y1), (x2-x1));
    Vector2d line(x1,y1,x2,y2);

    /// calculating line's perpendicular distance to ball
    Vector2d c1_circle (ball->getX() - x1, ball->getY() - y1);
    double perpendicular = c1_circle.projectionOn(leftNormal);

    line.sortPoints();
    /// Compare si la perpendiculaire generee depuis le cercle
    /// est plus petite que le rayon de celui-ci + l'epaisseur des murs
    /// True si collision, sinon false
    if( fabs(perpendicular) <= ball->getR() + Wall::THICKNESS ) {
        return CollisionDetection::withinLine(ball, line) ;
    }

    return false;
}

bool CollisionDetection::withinLine(Ball *ball, Vector2d vect) {
    double xmin = vect.get_x1()-ball->getR();
    double xmax = vect.get_x2()+ball->getR();
    double ymin = vect.get_y1();
    double ymax = vect.get_y2();

    if(ymin > ymax) {
        double tmp = ymin;
        ymin = ymax;
        ymax = tmp;
    }

    ymin -= ball->getR();
    ymax += ball->getR();

    if(vect.get_x1() != vect.get_x2()) {

        /// si la position x est entre x1 et x2
        if(ball->getX() >= xmin && ball->getX() <= xmax) {
            /// Dans le cas ou y1 != y2
            if(vect.get_y1() != vect.get_y2()) {

                /// si la position y est entre y1 et y2
                if(ball->getY() >= ymin && ball->getY() <= ymax)
                    return true;
            }   /// Cas ou y1 = y2
            else {
                return true;
            }
        }
    }
        /// x1 = x2, forcement y1 != y2
    else {
        if(ball->getY() >= ymin && ball->getY() <= ymax)
            return true;
    }
    return false;
}

bool CollisionDetection::findCollisions(Ball *ball, vector<Wall> walls, vector<Wall> &touchedWalls) {
    bool result = false;
    for(Wall current: walls) {
        if(intersect(ball, current)) {
            touchedWalls.push_back(current);
            result = true;
        }
    }
    return result;
}

bool CollisionDetection:: hasArrived(Ball *ball, cv::Point2d *point){
    return ball->getX() >= point->x - ball->getR()
           && ball->getX() <= point->x + ball->getR()
           && ball->getY() >= point->y - ball->getR()
           && ball->getY() <= point->y + ball->getR();
}

/////////////////////////////////////       TESTS         //////////////////////////////////////////////////

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


