#include <iostream>
#include "EdgeDetection.h"


using namespace std;
using namespace cv;


///tableau contenant les couleurs des coins
extern int colorCorner[2][4];



vector<vector<Point2f>> EdgeDetection::linesDetection(Mat img, int thresh){
    /// détection des contours avec Canny
    Mat imgCanny;
    Canny(img, imgCanny, 100, 200, 3);

    namedWindow("Canny",WINDOW_AUTOSIZE);
    imshow("Canny", imgCanny);

    /// detection des lignes dans le vect lines

    /// vecteur dans lequel sont stockées les lignes
    ///     lignes stockées sous la forme (x1,y1,x2,y2)
    vector<Vec4i> lines;
    /// houghLinesP(imgsource,
    /// vectdest,
    /// distance resolution en pixels
    /// angle resolution en rad
    /// seuil
    /// longueur min d'une ligne détectée
    /// max ecart entre pixels de la ligne)

    HoughLinesP(imgCanny, lines, 1, CV_PI/180, 100, 20, 25);

    /// tableau de couples de points
    vector<vector<Point2f>> vectLines;

    for( size_t i = 0; i < lines.size(); i++ ){
        /// récupération d'une ligne
        Vec4i l = lines[i];

        /// couple de points
        vector<Point2f> vectPoints ;
        vectPoints.push_back(Point2f(l[0], l[1]));
        vectPoints.push_back(Point2f(l[2], l[3]));

        /// ajout du couple au tableau
        vectLines.push_back(vectPoints) ;

        ///tracé de la ligne
        line( img, vectPoints[0], vectPoints[1], Scalar(0,0,255), 1, CV_AA);
    }

    return(vectLines);
}



vector<Point2d> EdgeDetection::getCorner(Mat img) {

    ///vector qui contient les coordonnées des coins
    vector<Point2d> coordCorner;
    ///déclaration et calcul de l'image hsv
    Mat hsv;
    cvtColor(img, hsv, CV_BGR2HSV);

    ///réglage des seuils de tolérance
    int toleranceh = 5;
    int tolerances = 30;

    Mat mask;
    for(int i = 0; i < 4; i++) {
        ///affichage de l'image suivant les seuils de tolérance
        inRange(hsv, Scalar(colorCorner[0][i] - toleranceh, colorCorner[1][i] - tolerances, 0), Scalar(colorCorner[0][i] + toleranceh, colorCorner[1][i] + tolerances, 255), mask);

        Mat kernel;
        kernel = getStructuringElement(2, Size(5,5), Point(2,2));
        erode(mask, mask, kernel);
        dilate(mask, mask, kernel);

        coordCorner.push_back(EdgeDetection::getBarycentre(mask));
        circle(img, EdgeDetection::getBarycentre(mask), 5, Scalar(0), 2, 8 ,0);
        namedWindow("1",WINDOW_AUTOSIZE);
        imshow("1", mask);
    }

   return coordCorner;

}

Point2d EdgeDetection::getBarycentre(Mat img) {

    int nbWhitePixels = 0;
    int xValue = 0;
    int yValue = 0;

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if((int)img.at<uchar>(i, j) == 255){
                nbWhitePixels++;
                xValue += j;
                yValue += i;
            }
        }
    }
    if(nbWhitePixels != 0) {
        xValue = xValue / nbWhitePixels;
        yValue = yValue / nbWhitePixels;
    }
    return Point2d(xValue, yValue);
}


