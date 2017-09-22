#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Transformation.h"

using namespace cv;
using namespace std;

/**
 *
 * @param matrix
 * @param outputQuad: The 4 points where the mapping is to be done , from top-left in clockwise order
 * @param size
 */
void Transformation::getHomographyMatrix(float matrix[], const cv::Point2f outputQuad[], const Size size) {

    // Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4];

    // Lambda Matrix
    Mat lambda(2, 4, CV_32FC1);

    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros(size.height, size.width, CV_32FC1);

    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    inputQuad[0] = Point2f(0, 0);
    inputQuad[1] = Point2f(size.width, 0);
    inputQuad[2] = Point2f(size.width, size.height);
    inputQuad[3] = Point2f(0, size.height);

    // The 4 points where the mapping is to be done , from top-left in clockwise order

    // Get the Perspective Transform Matrix i.e. lambda
    lambda = getPerspectiveTransform(inputQuad, outputQuad);

    cout << lambda << endl << endl;

    matrix[0] = (float)lambda.at<double>(0, 0);
    matrix[1] = (float)lambda.at<double>(0, 1);
    matrix[2] = 0.0;
    matrix[3] = (float)lambda.at<double>(0, 2);

    matrix[4] = (float)lambda.at<double>(1, 0);
    matrix[5] = (float)lambda.at<double>(1, 1);
    matrix[6] = 0.0;
    matrix[7] = (float)lambda.at<double>(1, 2);

    matrix[8] = 0.0;
    matrix[9] = 0.0;
    matrix[10] = 1.0;
    matrix[11] = 0.0;

    matrix[12] = (float)lambda.at<double>(2, 0);
    matrix[13] = (float)lambda.at<double>(2, 1);
    matrix[14] = 0.0;
    matrix[15] = (float)lambda.at<double>(2, 2);

}
