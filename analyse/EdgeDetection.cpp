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
void EdgeDetection::linesDetection(cv::Mat img, int thresh){
    /// détection des contours avec canny
    cv::Mat img2;
    cv::Canny(img, img2, 50, thresh, 3);

    /// detection des lignes dans le vect lines
  /* std::vector<cv::Vec2f> lines;
    HoughLines( img2, lines, 1, CV_PI/180, 100 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        cv::Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        cv::Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( img, pt1, pt2, cv::Scalar(0,0,255), 3, 8 );
    }*/
    std::vector<cv::Vec4i> lines;
    /// houghLinesP(imgsource,
    /// vectdest,
    /// distance resolution en pixels
    /// angle resolution en rad
    /// seuil
    /// min longeur
    /// max ecart entre pixels de la ligne)
    HoughLinesP(img2, lines, 1, CV_PI/180, 100, 50, 25 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::Vec4i l = lines[i];
        cv::line( img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, CV_AA);
    }
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