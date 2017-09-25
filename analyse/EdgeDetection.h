
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class EdgeDetection {

public:
    /* détecte les angles dans une image et les affiche
     * @params
     *      img : l'image où trouver les angles
     *      thresh : seuil de détection
     * */
    //static void cornersDetection(cv::Mat img, int thresh/*, cv::Point coord[]*/);
    static void colorDetection(cv::Mat img);

    /* détecte les lignes dans une image et renvoie les coordonnées des extrémités
     * @params
     *      img : l'image où trouver les lignes
     *      thresh : seuil de détection
     *
     * @return
     *      renvoie un vecteur de lignes
     *      les lignes sont de la forme Point(x1,y1) Point(x2,y2) qui sont les coordonnées des extrémités d'une ligne
     * */
    static std::vector<std::vector<cv::Point2f>> linesDetection(cv::Mat img, int thresh);

};


#endif //PROJET_AMAZED_EDGEDETECTION_H
