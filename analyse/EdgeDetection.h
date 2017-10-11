
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class EdgeDetection {

private:


/** Trie le vector de 4 points comme suit:
 *          le point particulier (couleur différente) en premier et les suivants dans le sens horaire
 *
 * @param coord le vector de coordonnées des quatres coins
 * @param imgHSV la frame courante en hsv
 * @return retourne le vector de 4 points trié
 */
    static std::vector<cv::Point2i> sortPoints(std::vector<cv::Point2i> coord, cv::Mat imgHSV);

public:

     /* Permet de trouver la teinte et la saturation des coins du plan
     */
    static void colorCalibration();

    /* détecte les couleurs d'une image
     * @params
     *      img : l'image où il faut détecter les couleurs
     */
    static std::vector<cv::Point2i> getCorner(cv::Mat img);

    /* détecte les lignes dans une image et renvoie les coordonnées des extrémités
     * @params
     *      img : l'image où trouver les lignes
     *      coordCorner : coin du plan
     * @return
     *      renvoie un vecteur de lignes
     *      les lignes sont de la forme Point(x1,y1) Point(x2,y2) qui sont les coordonnées des extrémités d'une ligne
     * */
    static std::vector<std::vector<cv::Point2i>> linesDetection(cv::Mat img, std::vector<cv::Point2i> coordCorner);

};


#endif //PROJET_AMAZED_EDGEDETECTION_H
