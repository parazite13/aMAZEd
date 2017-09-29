#include <iostream>
#include "EdgeDetection.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


vector<vector<Point2f>> EdgeDetection::linesDetection(Mat img, vector<Point2d> coordCorner){
    /// détection des contours avec Canny
    Mat imgCanny;
    Canny(img, imgCanny, 100, 200, 3);

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

    HoughLinesP(imgCanny, lines, 1, CV_PI/180, 50, 5, 10);

    /// tableau de couples de points
    vector<vector<Point2f>> vectLines;

    ///Initialisation du mask
    Mat mask = Mat::zeros(img.size(), CV_8UC1);

    ///Si on a 4 points alors
    ///On déssine un polygone avec ces 4 points dans le mask
    if(coordCorner.size() == 4) {
        ///Conversion des données pour utiliser la fonction fillPoly
        Point coord[1][4];
        coord[0][0] = coordCorner[0];
        coord[0][1] = coordCorner[1];
        coord[0][2] = coordCorner[2];
        coord[0][3] = coordCorner[3];
        ///Nombre de points
        int npt[] = {4};
        ///Pointeur de points
        const Point *ppt[1] = {coord[0]};

        fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255), 8);
    }



    for(Vec4i l : lines){

        /// couple de points
        vector<Point2f> vectPoints ;
        vectPoints.emplace_back(l[0], l[1]);
        vectPoints.emplace_back(l[2], l[3]);

        /// ajout du couple au tableau
        vectLines.push_back(vectPoints) ;

        ///tracé de la ligne
        if((int)mask.at<uchar>((int)vectPoints[0].y, (int)vectPoints[0].x) == 255 && (int)mask.at<uchar>((int)vectPoints[1].y, (int)vectPoints[1].x) == 255) {
            line( img, vectPoints[0], vectPoints[1], Scalar(0,0,255), 1, CV_AA);
        }
    }

    namedWindow("2",WINDOW_AUTOSIZE);
    imshow("2", img);

    return(vectLines);
}



vector<Point2d> EdgeDetection::getCorner(Mat img) {

    ///déclaration et calcul de l'image hsv
    Mat hsv;
    cvtColor(img, hsv, CV_BGR2HSV);

    circle(img, Point(300,300), 5, Scalar(0,0,255));
    cout << (int)hsv.at<Vec3b>(300, 300)[0] << endl;
    cout << (int)hsv.at<Vec3b>(300, 300)[1] << endl;

    ///réglage des seuils de tolérance
    int h = 80;
    int s = 250;
    int toleranceh = 10;
    int tolerances = 10;

    Mat mask;
    ///affichage de l'image suivant les seuils de tolérance
    inRange(hsv, Scalar(h-toleranceh, s-tolerances, 0), Scalar(h+toleranceh, s+tolerances, 255), mask);
    Mat kernel;
    kernel = getStructuringElement(2, Size(5,5), Point(2,2));
    erode(mask, mask, kernel);
    dilate(mask, mask, kernel);
    mask = ~mask;

    ///paramètre pour la détection des composantes connexes
    SimpleBlobDetector::Params params;

    params.minThreshold = 0;
    params.maxThreshold = 100;
    params.filterByArea = true;
    params.minArea = 500;
    params.maxArea = 10000;
    params.filterByCircularity = false;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
    std::vector<KeyPoint> keypoints;
    detector->detect( mask, keypoints );

    ///dessine les cercles correspondant aux composantes connexes
    drawKeypoints( mask, keypoints, mask, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    namedWindow("1",WINDOW_AUTOSIZE);
    imshow("1", mask);

    ///vector qui contient les coordonnées des coins
    vector<Point2d> coordCorner;

    if(keypoints.size() == 4){
        for(int i=0 ; i<4 ; i++ ){
            coordCorner.push_back(keypoints[i].pt);
        }
        /// ordre des points en fonction des exigences de la modélisation
        coordCorner = sortPoints(coordCorner);
    }
   return coordCorner;

}


/// fonction utilisée pour trier les points
bool sortByY(Point p1, Point p2){
    return p1.y>p2.y ;
}

vector<Point2d> EdgeDetection::sortPoints(vector<Point2d> coord){
    /// tri du y le plus grand au plus petit
    sort(coord.begin(), coord.end(), sortByY);
    /// comparaison des deux du bas et des deux du haut
    if(coord[0].x > coord[1].x) swap(coord[0],coord[1]);
    if(coord[2].x > coord[3].x) swap(coord[2],coord[3]);
    ///réarangement qui marche =)
    swap(coord[1],coord[2]);
    swap(coord[2],coord[3]);

    return coord;
}



