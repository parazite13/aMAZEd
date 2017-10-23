
#ifndef AMAZED_COLLISIONDETECTION_H
#define AMAZED_COLLISIONDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Ball.h"
#include "Vector2d.h"
#include "Wall.h"

class CollisionDetection {
private :

public  :
////////////////////////////    Collision detection    /////////////////////////////////

    /**
     * Le vecteur doit etre pre-traite avec sortPoints
     * Retourne vrai si la position x et y du cercle
     * et comprise entre le x1;y1 et x2;y2 du vecteur
     */
    static bool withinLine(Ball ball, Vector2d vect);

    static bool intersect(Ball ball, Wall wall);

    /**
     *  Parcours tous les murs (walls) et range dans touchedWalls les murs qui touchent la balle.
     *  La fonction renvoie false si aucun mur a ete touche, true sinon
     */
    static bool findCollisions(Ball ball, std::vector<Wall> walls, std::vector<Wall> &touchedWalls);


////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////    TESTS !     ///////////////////////////////////////////
    // Test
    struct Circle
    {
        float              radius;
        std::vector<float> velocity;
        cv::Point2d         center;
    };

    static Circle createCircle(float radius, cv::Point2d center) ;
    static void drawCircle(Circle circle);


    /**
     * Le vecteur doit etre pre-traite avec sortPoints
     * Retourne vrai si la position x et y du cercle
     * et comprise entre le x1;y1 et x2;y2 du vecteur
     */
    static bool withinLine(Circle circle, Vector2d vect);

    static bool intersect(Circle circle, Vector2d vect);

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

///////////////////////////////////////////////////////////////////////////////////////

};


#endif //AMAZED_COLLISIONDETECTION_H