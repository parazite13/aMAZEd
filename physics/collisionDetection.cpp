
#include <GL/glut.h>
#include <iostream>
#include "collisionDetection.h"

using namespace std;

/// Hitboxes carrees contre Hitboxes carrees
bool collisionDetection::AABBvsAABB( Hitbox obj1, Hitbox obj2 ) {

    // Exit with no intersection if found separated along an axis
    if(obj1.max.x < obj2.min.x || obj1.min.x > obj2.max.x) return false ;
    if(obj1.max.y < obj2.min.y || obj1.min.y > obj2.max.y) return false ;

    // No separating axis found, therefor there is at least one overlapping axis
    return true ;
}
/**
 * Determine si 2 cercles a et b se rentrent dedans
 * On calcule la distance en elevant tout au carre pour eviter de passer par un sqrt
 * @param a
 * @param b
 * @return true si collision, false sinon
 */
bool collisionDetection::CirclevsCircleOptimized( collisionDetection::Circle a, collisionDetection::Circle b ) {
    float r = a.radius + b.radius ;
    r *= r ;
    return r >= ((a.center.x + b.center.x)*(a.center.x + b.center.x) + (a.center.y + b.center.y)*(a.center.y + b.center.y));
}

// A adapter
bool collisionDetection::CirclevsAABB(Circle circle, Hitbox box) {
    // get box closest point to sphere center by clamping
    double x = std::max( box.min.x, std::min(circle.center.x, box.max.x) );
    double y = std::max( box.min.y, std::min(circle.center.y, box.max.y) );


    // Distance between the center of the sphere and the closest box's point to it
    double distance = (x - circle.center.x) * (x - circle.center.x) +
                     (y - circle.center.y) * (y - circle.center.y) ;

    return distance < circle.radius*circle.radius;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool collisionDetection::onSegment(cv::Point2d p, cv::Point2d q, cv::Point2d r) {
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
int collisionDetection::orientation(cv::Point2d p, cv::Point2d q, cv::Point2d r) {
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
bool collisionDetection::doIntersect(cv::Point2d circleCenter, cv::Point2d nextPosition, Hitbox box) {

    vector<cv::Point2d> nearestEdge = collisionDetection::selectNearestEdge(circleCenter, box);
    cv::Point2d p2 = nearestEdge[0];
    cv::Point2d q2 = nearestEdge[1];

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(circleCenter, nextPosition, p2);
    int o2 = orientation(circleCenter, nextPosition, q2);
    int o3 = orientation(p2, q2, circleCenter);
    int o4 = orientation(p2, q2, nextPosition);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && collisionDetection::onSegment(circleCenter, p2, nextPosition)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && collisionDetection::onSegment(circleCenter, q2, nextPosition)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && collisionDetection::onSegment(p2, circleCenter, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && collisionDetection::onSegment(p2, nextPosition, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

vector<cv::Point2d> collisionDetection::selectNearestEdge(cv::Point2d circleCenter, Hitbox box) {
    vector<cv::Point2d> nearestEdge;
    cv::Point2d min1, min2;

    // A(xa, ya) et B(xb, yb) -> distance AB² = (xb-xa)² + (yb-ya)²
    double distance;
    double distanceMin = 99999999;
    for(cv::Point2d point : box.vertices) {
        distance = (point.x - circleCenter.x)*(point.x - circleCenter.x) + (point.y -circleCenter.y)*(point.x - circleCenter.x);
        if(distance < distanceMin) {
            min1 = point;
            distanceMin = distance;
        }
    }

    distanceMin = 99999999;
    for(cv::Point2d point : box.vertices) {
        distance = (point.x - circleCenter.x) + (point.y -circleCenter.y);
        if(distance < distanceMin && point != min1) {
            min2 = point;
            distanceMin = distance;
        }
    }

    nearestEdge.push_back(min1);
    nearestEdge.push_back(min2);

    return nearestEdge;
}

/// Test !
/// Separer le dessin de la creation sinon lors de l'affichage, les points reviennent a leur position d'origine dans l'objet lors d'un deplacement
collisionDetection::Circle collisionDetection::createCircle(float radius, cv::Point2d center) {
    collisionDetection::Circle circle = collisionDetection::Circle();
    circle.radius = radius;
    circle.velocity.push_back(0) ;
    circle.velocity.push_back(0) ;
    circle.center.x = center.x;
    circle.center.y = center.y;

    return circle;
}

/// Test !
void collisionDetection::drawCircle(collisionDetection::Circle circle){
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

float collisionDetection::dotProduct(vector<float> v1, vector<float> v2) {
    //              x1   * x2   + y1   * y2
    float result = v1[0]*v2[0] + v1[1]*v2[1];

    return result;
}

/*
 *  Calcule les forces de reactions suite a une collision entre la balle et un mur
 */
void collisionDetection::ResolveCollision( collisionDetection::Circle a, collisionDetection::Circle b ) {
    // Calculate relative velocity : b.velocity - a.velocity
    vector<float> rv;
    rv.push_back(b.velocity[0] - a.velocity[0]) ;
    rv.push_back(b.velocity[1] - a.velocity[1]) ;

    // Calculate relative velocity in terms of the normal direction
    //float velAlongNormal = dotProduct( rv, normal ) ;
    float velAlongNormal = dotProduct( rv, rv ) ;

    // Do not resolve if velocities are separating
    if(velAlongNormal > 0)
        return;

    // Calculate restitution
    float e = min( a.restitution, b.restitution) ;

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    //j /= 1 / a.mass + 1 / b.mass;

    // Apply impulse
    //Vec2 impulse = j * normal;
    //a.velocity -= 1 / a.mass * impulse;
    //b.velocity += 1 / b.mass * impulse;
}


