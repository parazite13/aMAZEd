
#include "EdgeDetection.h"

void EdgeDetection::cornersDetection(cv::Mat img, int thresh/*, cv::Point coord[]*/){
    cv::Mat imgGrey;
    int nbCorner = 0;
    ///conversion de l'image en niveaux de gris
    cv::cvtColor( img, imgGrey, CV_BGR2GRAY );

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

               /* coord[nbCorner] = cv::Point(i, j);
                nbCorner++;*/

                circle( img, cv::Point( i, j ), 5,  cv::Scalar(0), 2, 8, 0 );

            }
        }
    }
}