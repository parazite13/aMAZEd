
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class EdgeDetection {

private:
    cv::Mat hsv;
    static void getObjectColor(int event, int x, int y, int flags, void *param = NULL);

public:
    /* détecte les angles dans une image et les affiche
     * @params
     *      img : l'image où trouver les angles
     *      thresh : seuil de détection
     * */
    //static void cornersDetection(cv::Mat img, int thresh/*, cv::Point coord[]*/);

    /* détecte les couleurs d'une image
     * @params
     *      img : l'image où il faut détecter les couleurs
     */
    void colorDetection(cv::Mat img);

    /* détecte les lignes dans une image et les affiche
     * @params
     *      img : l'image où trouver les lignes
     *      thresh : seuil de détection
     * */
    void linesDetection(cv::Mat img, int thresh);


};


#endif //PROJET_AMAZED_EDGEDETECTION_H
