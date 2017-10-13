#include <iostream>
#include "EdgeDetection.h"
#include "opencv2/opencv.hpp"
#include "../stream/CameraStream.h"

using namespace std;
using namespace cv;

///Récupération du flux de la caméra
CameraStream extern cameraStream;

///Réglage de la teinte et de la saturation pour le vert
int hGreen = 0;
int sGreen = 230;

///Réglage de la teinte et de la saturation pour le mauve
int hPink = 0;
int sPink = 140;


///Fonction permettant la calibration de la couleur
void EdgeDetection::colorCalibration(){
    std::vector<Point2i> keypoints;
    Mat img;
    int minh = 50;
    int maxh = 95;
    ///Tant que l'on a pas 3 points, on cherche à calibrer le vert
    while(keypoints.size() < 3){
        ///Calibration de la teinte
        hGreen = (hGreen-minh+7)%(maxh-minh)+minh;
        img = cameraStream.getCurrentFrame();
        keypoints = getCorner(img);
        ///Quand on à fait une boucle sur la teinte, on baisse la saturation
        if(hGreen == minh){
            sGreen = sGreen-30;
            if(sGreen <= 120){
                sGreen = 210;
            }
        }
    }

    minh = 150;
    maxh = 175;
    ///Tant que l'on a pas le 4eme point, on cherche à calibrer le mauve
    while(keypoints.size() < 4){
        ///Calibration de la teinte
        hPink = (hPink-minh+7)%(maxh-minh)+minh;
        img = cameraStream.getCurrentFrame();
        keypoints = getCorner(img);
        ///Quand on à fait une boucle sur la teinte, on baisse la saturation
        if(hPink == minh){
            sPink = sPink-30;
            if(sPink <= 80){
                sPink = 140;
            }
        }
    }
}

vector<vector<Point2i>> EdgeDetection::linesDetection(Mat img, vector<Point2i> coordCorner){
    /// détection des contours avec Canny
    Mat imgCanny;
    Canny(img, imgCanny, 100, 300, 3);

    /// detection des lignes dans le vect lines

    /// vecteur dans lequel sont stockées les lignes
    ///     lignes stockées sous la forme (x1,y1,x2,y2)
    vector<Vec4i> lines;
    /// houghLinesP(imgsource,
    /// vectdest,
    /// distance resolution en pixels
    /// angle resolution en rad
    /// seuil :The minimum number of intersections to “detect” a line
    /// longueur min d'une ligne détectée
    /// max ecart entre pixels de la ligne)

    HoughLinesP(imgCanny, lines, 1, CV_PI/180, 80, 20, 15);

    /// tableau de couples de points
    vector<vector<Point2i>> vectLines;

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
        vector<Point2i> vectPoints ;
        vectPoints.emplace_back(l[0], l[1]);
        vectPoints.emplace_back(l[2], l[3]);



        ///tracé de la ligne
        if((int)mask.at<uchar>(vectPoints[0].y, vectPoints[0].x) == 255 && (int)mask.at<uchar>(vectPoints[1].y, vectPoints[1].x) == 255) {
            /// ajout du couple au tableau
            vectLines.push_back(vectPoints) ;
            line( img, vectPoints[0], vectPoints[1], Scalar(0,0,255), 1, CV_AA);
        }
    }


    return(vectLines);
}



vector<Point2i> EdgeDetection::getCorner(Mat img) {

    ///Liste contenant les points des coins du plan
    std::vector<KeyPoint> keypoints;
    ///Masque
    Mat mask;

    ///Déclaration et calcul de l'image hsv
    Mat hsv;
    cvtColor(img, hsv, CV_BGR2HSV);

//    ///Permet de voir la couleur du pixel 300/300
//    circle(img, Point(300, 300), 5, Scalar(0, 0, 255));
//    cout << "h = " << (int) hsv.at<Vec3b>(300, 300)[0] << endl;
//    cout << "s = " << (int) hsv.at<Vec3b>(300, 300)[1] << endl;
//    cout << "l = " << (int) hsv.at<Vec3b>(300, 300)[2] << endl;
//
//    namedWindow("3",WINDOW_AUTOSIZE);
//    imshow("3", img);


    ///Réglage des seuils de tolérance
    int toleranceh = 12;
    int tolerances = 45;

    ///Affichage de l'image suivant les seuils de tolérance
    inRange(hsv, Scalar(hGreen - toleranceh, sGreen - tolerances, 50), Scalar(hGreen + toleranceh, sGreen + tolerances, 255), mask);

    /// On inverse le mask
    mask = ~mask;

    ///On créé un masque temporraire permettant d'enlever les trous à l'interrieure d'une composante connexe
    Mat maskTemp = mask.clone();
    cv::floodFill(maskTemp, cv::Point(0, 0), CV_RGB(0, 0, 0));
    maskTemp = ~maskTemp;
    mask = maskTemp & mask;


    ///Paramètre pour la détection des composantes connexes
    SimpleBlobDetector::Params params;

    params.minThreshold = 0;
    params.maxThreshold = 100;
    params.filterByArea = true;
    params.minArea = 400;
    params.maxArea = 10000;
    params.filterByCircularity = false;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

    detector->detect(mask, keypoints);


    ///Dessine les cercles correspondant aux composantes connexes
    drawKeypoints( mask, keypoints, mask, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    ///On affiche le premier mask qui correspond au vert
    namedWindow("1",WINDOW_AUTOSIZE);
    imshow("1", mask);

    ///Vector qui contient les coordonnées des coins
    vector<Point2i> coordCorner;

    if(keypoints.size() == 3){
        for(int i=0 ; i<3 ; i++ ){
            coordCorner.push_back(keypoints[i].pt);
        }
    }

        /// si plus de 3 composantes connexes trouvées on prends les 4 plus grosses
    else if(keypoints.size() > 3){
        for(int i=0 ; i<3 ; i++ ){
            int imax = 0;
            for(int j = 1 ; j< keypoints.size() ; j++) {
                if (keypoints[j].size > keypoints[imax].size) {
                    imax = j;
                }
            }
            coordCorner.push_back(keypoints[imax].pt);
            keypoints[imax].size = 0;
        }
    }

    ///Réglage des seuils de tolérance
    toleranceh = 12;
    tolerances = 60;

    ///affichage de l'image suivant les seuils de tolérance
    inRange(hsv, Scalar(hPink - toleranceh, sPink - tolerances, 50), Scalar(hPink + toleranceh, sPink + tolerances, 255), mask);

    /// On inverse le mask
    mask = ~mask;

    ///On créé un masque temporraire permettant d'enlever les trous à l'interrieure d'une composante connexe
    maskTemp = mask.clone();
    cv::floodFill(maskTemp, cv::Point(0, 0), CV_RGB(0, 0, 0));
    maskTemp = ~maskTemp;
    mask = maskTemp & mask;

    keypoints.clear();
    detector->detect(mask, keypoints);
    drawKeypoints( mask, keypoints, mask, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    namedWindow("2",WINDOW_AUTOSIZE);
    imshow("2", mask);


    if(keypoints.size() > 0){
        int imax = 0;
        for(int j = 1 ; j< keypoints.size() ; j++) {
            if (keypoints[j].size > keypoints[imax].size) {
                imax = j;
            }
        }
        coordCorner.push_back(keypoints[imax].pt);
        keypoints[imax].size = 0;
    }

    /// ordre des points en fonction des exigences de la modélisation
    if(coordCorner.size() == 4) {
        coordCorner = sortPoints(coordCorner, hsv);
    }

    return coordCorner;


}


/// fonction utilisée pour trier les points
bool sortByY(Point p1, Point p2){
    return p1.y>p2.y ;
}

vector<Point2i> EdgeDetection::sortPoints(vector<Point2i> coord, Mat imgHSV){
    /// tri du y le plus grand au plus petit
    sort(coord.begin(), coord.end(), sortByY);
    /// comparaison des deux du bas et des deux du haut
    if(coord[0].x > coord[1].x) swap(coord[0],coord[1]);
    if(coord[2].x > coord[3].x) swap(coord[2],coord[3]);
    ///réarangement qui marche =)
    swap(coord[1],coord[2]);
    swap(coord[2],coord[3]);

    // hGreen+-12  sGreen+-45
    int compt =0;// sécurité
    while( !((int)imgHSV.at<Vec3b>(coord[0].y,coord[0].x)[0] > (hPink - 12)
            && ((int)imgHSV.at<Vec3b>(coord[0].y,coord[0].x)[0] < (hPink + 12))
            && ((int)imgHSV.at<Vec3b>(coord[0].y,coord[0].x)[1] > (sPink - 45))
            && ((int)imgHSV.at<Vec3b>(coord[0].y,coord[0].x)[1] < (sPink + 45))
        ) && (compt < 5)){ //tant que le point unique n'est pas en premier on rotato

        std::rotate(coord.begin(),coord.begin()+1,coord.end());
        compt++;
    }

    return coord;
}



