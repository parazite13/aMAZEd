#include <iostream>
#include "EdgeDetection.h"


using namespace std;
using namespace cv;

///Récupération de l'image d'origine
extern Mat frame;
///Déclaration de la teinte (h) et de la saturation (s) pour le format hsv
int h, s;
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

    vector<Mat> canaux;

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

                    //cout << (int)canaux[0].at<uchar>(j,i) << endl;

                    //std::cout << (int)canaux[0].at<uchar>(j,i) << std::endl;
                }
            }
        }
    }
}

*/

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



void EdgeDetection::colorDetection(Mat img) {
    ///Conversion en hsv
    cvtColor(img, this->hsv, CV_BGR2HSV);

    ///Création d'un masque
    Mat mask;


    ///réglage des seuils de tolérance
    int toleranceh = 10;
    int tolerances = 30;
    ///affichage de l'image suivant les seuils de tolérance
    inRange(hsv, Scalar(h-toleranceh-1, s-tolerances, 0), Scalar(h+toleranceh -1, s+tolerances, 255), mask);

    ///Permet de suprimer les parasites
    Mat kernel;
    kernel = getStructuringElement(2, Size(5,5), Point(2,2));

    dilate(mask, mask, kernel);
    erode(mask, mask, kernel);

    ///Action bouton souri
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
        h = hsv.at<Vec3b>(y, x)[0];
        s = hsv.at<Vec3b>(y, x)[1];
    }
}


