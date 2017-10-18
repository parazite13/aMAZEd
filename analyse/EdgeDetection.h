
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../stream/CameraStream.h"

class EdgeDetection {

private:

    CameraStream *cameraStream;

/** Trie le vector de 4 points comme suit:
 *          le point particulier (couleur différente) en premier et les suivants dans le sens horaire
 *
 * @param coord le vector de coordonnées des quatres coins
 * @param imgHSV la frame courante en hsv
 * @return retourne le vector de 4 points trié
 */
    std::vector<cv::Point2i> sortPoints(std::vector<cv::Point2i> coord, cv::Mat imgHSV);

public:

    explicit EdgeDetection(CameraStream *cameraStream);

     /** Permet de trouver la teinte et la saturation des coins du plan
     */
    cv::Mat colorCalibration();

    /** détecte les couleurs d'une image
     * @params
     *      img : l'image où il faut détecter les couleurs
     */
    std::vector<cv::Point2i> getCorner(cv::Mat img);

    /** détecte les lignes dans une image et renvoie les coordonnées des extrémités
     * @params
     *      img : l'image où trouver les lignes
     *      coordCorner : coin du plan
     * @return
     *      renvoie un vecteur de lignes
     *      les lignes sont de la forme Point(x1,y1) Point(x2,y2) qui sont les coordonnées des extrémités d'une ligne
     * */
    std::vector<std::vector<cv::Point2i>> linesDetection(cv::Mat img, std::vector<cv::Point2i> coordCorner);

};


#endif //PROJET_AMAZED_EDGEDETECTION_H
