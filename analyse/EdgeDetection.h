
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class EdgeDetection {

public:
    /* détecte les angles dans une image
     * @params
     *      img : l'image où trouver les angles
     *      thresh : seuil de détection
     * */
    static void cornersDetection(cv::Mat img, int thresh/*, cv::Point coord[]*/);

};


#endif //PROJET_AMAZED_EDGEDETECTION_H
