#include <iostream>
#include "EdgeDetection.h"

using namespace cv;

extern Mat frame;

/*
void EdgeDetection::cornersDetection(Mat img, int thresh){
    Mat imgGrey;
    Mat imgHLS;
    int nbCorner = 0;
    ///conversion de l'image en niveaux de gris
    cvtColor( img, imgGrey, CV_BGR2GRAY );
    ///conversion de RGB à HLS
    cvtColor( img, imgHLS, CV_BGR2HLS );

    ///On split les canaux
    std::vector<Mat> canaux;
    split(imgHLS,canaux);


    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( img.size(), CV_32FC1 );  //init

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    /// Detecting corners
    cornerHarris( imgGrey, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

    /// Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    for( int j = 0; j < dst_norm.rows ; j++ ){
        for( int i = 0; i < dst_norm.cols; i++ ){
            if( (int) dst_norm.at<float>(j,i) > thresh ){

                coord[nbCorner] = Point(i, j);
                nbCorner++;

                if((int)canaux[1].at<uchar>(j,i) <= 185 && (int)canaux[1].at<uchar>(j,i) >= 165) {
                    circle(img, Point(i, j), 5, Scalar(0), 2, 8, 0);
                    //std::cout << (int)canaux[0].at<uchar>(j,i) << std::endl;
                }

            }
        }
    }
}

*/
void EdgeDetection::linesDetection(Mat img, int thresh){
    /// détection des contours avec canny
    Mat img2;
    Canny(img, img2, 50, thresh, 3);

    /// detection des lignes dans le vect lines
  /* std::vector<Vec2f> lines;
    HoughLines( img2, lines, 1, CV_PI/180, 100 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( img, pt1, pt2, Scalar(0,0,255), 3, 8 );
    }*/
    std::vector<Vec4i> lines;
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
        Vec4i l = lines[i];
        line( img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
    }
}



void EdgeDetection::colorDetection(Mat img) {
    ///Conversion en hsv
    cvtColor(img, this->hsv, CV_BGR2HSV);

    ///Création d'un masque
    Mat mask;

    ///4 variables pour détecter les seuils de h et s
    int h = 175;
    int s = 80;
    int toleranceh = 15;
    int tolerances = 40;

    ///inRange permet de parcourir toute l'image hsv
    ///avec les deux scalars on définit l'intervalle de détection de couleur
    ///le résultat est conservé dans une image binaire appelée mask
    inRange(this->hsv, Scalar(h-toleranceh, 0, 0), Scalar(h+toleranceh, 255, 255), mask);

    ///Permet de suprimer les parasites
    Mat kernel;
    kernel = getStructuringElement(2, Size(5,5), Point(2,2));

    dilate(mask, mask, kernel);
    erode(mask, mask, kernel);

    ///
    setMouseCallback("Frame", getObjectColor);


    ///affiche l'image en noir et blanc
    namedWindow("1",WINDOW_AUTOSIZE);
    imshow("1", mask);

}

void EdgeDetection::getObjectColor(int event, int x, int y, int flags, void *param){
    Mat hsv;
    ///Conversion en hsv
    cvtColor(frame, hsv, CV_BGR2HSV);
    if(event == CV_EVENT_LBUTTONUP) {
        std::cout << hsv.at<Vec3b>(y, x) << std::endl;
    }
}


