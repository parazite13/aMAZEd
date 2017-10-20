//
// Created by Fabien on 2017/10/20.
//

#ifndef AMAZED_COLLISIONDETECTION_H
#define AMAZED_COLLISIONDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Hitbox.h"

class collisionDetection {
private :

public  :
//////////////////////////   Structures + constructeurs   ////////////////////////////////
    // Test
    struct Circle
    {
        float              radius;
        std::vector<float> velocity;
        cv::Point2d         center;
        int                restitution;
    };

    static Circle createCircle(float radius, cv::Point2d center) ;
    static void drawCircle(Circle circle);

////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////    Collision detection    /////////////////////////////////
    static bool AABBvsLine(Hitbox square, cv::Point2d begin, cv::Point2d end);

    /**
     *  Determine si les 2 objets passes en argument sont en contact
     * @return true si collision, false sinon
     */
    static bool AABBvsAABB(Hitbox obj1, Hitbox obj2);

    static bool CirclevsCircleOptimized( Circle a, Circle b ) ;

    static bool CirclevsAABB(Circle circle, Hitbox box);

    static bool onSegment(cv::Point2d p, cv::Point2d q, cv::Point2d r);

    static int orientation(cv::Point2d p, cv::Point2d q, cv::Point2d r);
    /**
     * Determine si circle traverse square en allant jusqu'a nextPosition via un ray cast
     * True si oui, false sinon
     */
    static bool doIntersect(cv::Point2d circleCenter, cv::Point2d nextPosition, Hitbox box);

    /// selectionne l'arete d'une hitbox la plus proche par rapport a la boule
    /// L'arete est representee par ses 2 points, renvoyes dans un vector
    static std::vector<cv::Point2d> selectNearestEdge(cv::Point2d circleCenter, Hitbox box);


////////////////////////////////////////////////////////////////////////////////////////


    /**
     *  Les vecteurs doivent etre de la meme taille
     * @param v1
     * @param v2
     * @return Le produit vectoriel des 2 vecteurs
     */
    static float dotProduct(std::vector<float> v1, std::vector<float> v2);

    static void ResolveCollision( Circle a, Circle b ) ;
};


#endif //AMAZED_COLLISIONDETECTION_H
