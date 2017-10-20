

#ifndef AMAZED_HITBOX_H
#define AMAZED_HITBOX_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Hitbox {
    public :
        cv::Point2d vertices[4];
        int         restitution;

    // Hitbox : min designe le coin en haut a gauche, max le coin en bas a droite
    // utilise si on utilise des hitboxes de type AABB
        cv::Point2d min;
        cv::Point2d max;

        Hitbox(cv::Point2d bot_left, cv::Point2d bot_right, cv::Point2d top_left, cv::Point2d top_right) ;
        void drawSquare();
};


#endif //AMAZED_HITBOX_H
