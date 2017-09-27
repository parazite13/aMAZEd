#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include "Transformation.h"

using namespace cv;
using namespace std;

Transformation::Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size) {

    this->size = size;

    computeHomographyMatrix(edgeCoordinate);

}

void Transformation::computeHomographyMatrix(std::vector<cv::Point2d> &edgeCoordinate) {

    // The 4 points that select quadilateral on the input , from top-left edge clockwise order
    // These four pts are the sides of the rect box used as input
    vector<Point2f> realWorldEdgeCoordinate = vector<Point2f>();
    realWorldEdgeCoordinate.emplace_back(0.0f, 0.0f);
    realWorldEdgeCoordinate.emplace_back(0.0f, 1.0f);
    realWorldEdgeCoordinate.emplace_back(1.0f, 1.0f);
    realWorldEdgeCoordinate.emplace_back(1.0f, 0.0f);

    vector<Point2f> edgeFloat;
    for (auto &edge : edgeCoordinate) {
        edgeFloat.emplace_back(((float)edge.x / this->size.width), 1.0f - ((float)edge.y / this->size.height));
    }

    this->H = findHomography(realWorldEdgeCoordinate, edgeFloat, RANSAC);

}

const void Transformation::getHomography(float *matrix) const {

    matrix[0] = (float)this->H.at<double>(0, 0);
    matrix[1] = (float)this->H.at<double>(1, 0);
    matrix[2] = 0.0f;
    matrix[3] = (float)this->H.at<double>(2, 0);

    matrix[4] = (float)this->H.at<double>(0, 1);
    matrix[5] = (float)this->H.at<double>(1, 1);
    matrix[6] = 0.0f;
    matrix[7] = (float)this->H.at<double>(2, 1);

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = (float)this->H.at<double>(0, 2);
    matrix[13] = (float)this->H.at<double>(1, 2);
    matrix[14] = 0.0f;
    matrix[15] = (float)this->H.at<double>(2, 2);
}
