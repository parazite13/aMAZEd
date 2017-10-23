
#ifndef AMAZED_COLLISIONDETECTION_H
#define AMAZED_COLLISIONDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Ball.h"
#include "Vector2d.h"

class CollisionDetection {
private :

public  :
//////////////////////////   Structures + constructeurs   ////////////////////////////////
    // Test
    struct Circle
    {
        float              radius;
        std::vector<float> velocity;
        cv::Point2d         center;
    };

    static Circle createCircle(float radius, cv::Point2d center) ;
    static void drawCircle(Circle circle);

////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////    Collision detection    /////////////////////////////////

    static bool intersect(Ball ball);

    static bool intersect(Circle circle, double x1, double y1, double x2 ,double y2);

    static bool onSegment(cv::Point2d p, cv::Point2d q, cv::Point2d r);

    static int orientation(cv::Point2d p, cv::Point2d q, cv::Point2d r);
    /**
     * Determine si circle traverse square en allant jusqu'a nextPosition via un ray cast
     * True si oui, false sinon
     */
    //static bool doIntersect(cv::Point2d circleCenter, cv::Point2d nextPosition, Hitbox box);

    /// selectionne l'arete d'une hitbox la plus proche par rapport a la boule
    /// L'arete est representee par ses 2 points, renvoyes dans un vector
    //static std::vector<cv::Point2d> selectNearestEdge(cv::Point2d circleCenter, Hitbox box);


////////////////////////////////////////////////////////////////////////////////////////

};


#endif //AMAZED_COLLISIONDETECTION_H
