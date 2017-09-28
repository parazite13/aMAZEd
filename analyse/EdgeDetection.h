
#ifndef PROJET_AMAZED_EDGEDETECTION_H
#define PROJET_AMAZED_EDGEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class EdgeDetection {

private:


    /**
     * Trie le vector de points selon l'odre désiré par la team modelisation
     * @param coord vecteur de 4 Points !!! doit être obligatoirement de 4 !!! sinon CRASH
     * @return vecteur avec les 4 points ordonnés:
     *         0 : bas gauche
     *         1 : haut gauche
     *         2 : haut droite
     *         3 : bas droite
     */
    static std::vector<cv::Point2d> sortPoints(std::vector<cv::Point2d> coord);

public:

    /* détecte les couleurs d'une image
     * @params
     *      img : l'image où il faut détecter les couleurs
     */
    static std::vector<cv::Point2d> getCorner(cv::Mat img);

    /* détecte les lignes dans une image et renvoie les coordonnées des extrémités
     * @params
     *      img : l'image où trouver les lignes
     *      coordCorner : coin du plan
     * @return
     *      renvoie un vecteur de lignes
     *      les lignes sont de la forme Point(x1,y1) Point(x2,y2) qui sont les coordonnées des extrémités d'une ligne
     * */
    static std::vector<std::vector<cv::Point2f>> linesDetection(cv::Mat img, std::vector<cv::Point2d> coordCorner);

};


#endif //PROJET_AMAZED_EDGEDETECTION_H
