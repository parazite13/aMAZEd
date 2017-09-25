#include <iostream>
#include "EdgeDetection.h"

/*
void EdgeDetection::cornersDetection(cv::Mat img, int thresh){
    cv::Mat imgGrey;
    cv::Mat imgHLS;
    int nbCorner = 0;
    ///conversion de l'image en niveaux de gris
    cv::cvtColor( img, imgGrey, CV_BGR2GRAY );
    ///conversion de RGB à HLS
    cv::cvtColor( img, imgHLS, CV_BGR2HLS );

    ///On split les canaux
    std::vector<cv::Mat> canaux;
    cv::split(imgHLS,canaux);


    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros( img.size(), CV_32FC1 );  //init

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    /// Detecting corners
    cornerHarris( imgGrey, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

    /// Normalizing
    normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    for( int j = 0; j < dst_norm.rows ; j++ ){
        for( int i = 0; i < dst_norm.cols; i++ ){
            if( (int) dst_norm.at<float>(j,i) > thresh ){

                coord[nbCorner] = cv::Point(i, j);
                nbCorner++;

                if((int)canaux[1].at<uchar>(j,i) <= 185 && (int)canaux[1].at<uchar>(j,i) >= 165) {
                    circle(img, cv::Point(i, j), 5, cv::Scalar(0), 2, 8, 0);
                    //std::cout << (int)canaux[0].at<uchar>(j,i) << std::endl;
                }

            }
        }
    }
}

*/
std::vector<std::vector<cv::Point2f>> EdgeDetection::linesDetection(cv::Mat img, int thresh){
    /// détection des contours avec Canny
    cv::Mat imgCanny;
    cv::Canny(img, imgCanny, 100, 200, 3);

//    cv::namedWindow("Canny",cv::WINDOW_AUTOSIZE);
//    cv::imshow("Canny", imgCanny);

    /// detection des lignes dans le vect lines

    /// vecteur dans lequel sont stockées les lignes
    ///     lignes stockées sous la forme (x1,y1,x2,y2)
    std::vector<cv::Vec4i> lines;

    /// houghLinesP(imgsource,
    /// vectdest,
    /// distance resolution en pixels
    /// angle resolution en rad
    /// seuil
    /// longueur min d'une ligne détectée
    /// max ecart entre pixels de la ligne)
    HoughLinesP(imgCanny, lines, 1, CV_PI/180, 100, 20, 25);

    /// tableau de couples de points
    std::vector<std::vector<cv::Point2f>> vectLines;

    for( size_t i = 0; i < lines.size(); i++ ){
        /// récupération d'une ligne
        cv::Vec4i l = lines[i];

        /// couple de points
        std::vector<cv::Point2f> vectPoints ;
        vectPoints.push_back(cv::Point2f(l[0], l[1]));
        vectPoints.push_back(cv::Point2f(l[2], l[3]));

        /// ajout du couple au tableau
        vectLines.push_back(vectPoints) ;

        ///tracé de la ligne
        cv::line( img, vectPoints[0], vectPoints[1], cv::Scalar(0,0,255), 1, CV_AA);
    }

    return(vectLines);
}



void EdgeDetection::colorDetection(cv::Mat img) {
    ///Conversion en hsv
    cv::Mat hsv;
    cv::cvtColor(img, hsv, CV_BGR2HSV);

    ///Création d'un masque
    cv::Mat mask;

    ///Si le pixel de l'image original à un h et un s proche de celui recharché on colorie l'image "mask" en blanc, autrement en noir
    int h = 346;
    int s = 83;
    int toleranceh = 4;
    int tolerances = 17;
    cv::inRange(hsv, cv::Scalar(h-toleranceh-1, s-tolerances, 0), cv::Scalar(h+toleranceh -1, s+tolerances, 255), mask);


    cv::namedWindow("1",cv::WINDOW_AUTOSIZE);
    cv::imshow("1", mask);

}