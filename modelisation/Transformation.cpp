#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include "Transformation.h"

using namespace cv;
using namespace std;

Transformation::Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, float near, float far) {
    this->size = size;
    this->near = near;
    this->far = far;

    this->homography = Mat(3, 3, CV_32FC1);
    computeHomographyMatrix(edgeCoordinate);

    this->intrinsic = Mat(3, 3, CV_32FC1);
    computeIntrinsicMatrix();

    this->extrinsic = Mat(3, 4, CV_32FC1);
    computeExtrinsicMatrix();

}


void Transformation::getProjectionMatrix(float *matrix) {

    matrix[0] = this->intrinsic.at<float>(0, 0);
    matrix[1] = this->intrinsic.at<float>(0, 1);
    matrix[2] = this->intrinsic.at<float>(0, 2);
    matrix[3] = 0.0;

    matrix[4] = this->intrinsic.at<float>(1, 0);
    matrix[5] = this->intrinsic.at<float>(1, 1);
    matrix[6] = this->intrinsic.at<float>(1, 2);
    matrix[7] = 0.0;

    matrix[8] = 0.0;
    matrix[9] = 0.0;
    matrix[10] = this->near + this->far;
    matrix[11] = this->near * this->far;

    matrix[12] = this->intrinsic.at<float>(2, 0);
    matrix[13] = this->intrinsic.at<float>(2, 1);
    matrix[14] = this->intrinsic.at<float>(2, 2);
    matrix[15] = 0.0;

    cout << "Projection\n";

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++) {
            cout << matrix[i * 4 + j] << ", ";
        }
        cout << endl;
    }

}


void Transformation::getModelviewMatrix(float *matrix) {

    matrix[0] = this->extrinsic.at<float>(0, 0);
    matrix[1] = this->extrinsic.at<float>(0, 1);
    matrix[2] = this->extrinsic.at<float>(0, 2);
    matrix[3] = this->extrinsic.at<float>(0, 3);

    matrix[4] = this->extrinsic.at<float>(1, 0);
    matrix[5] = this->extrinsic.at<float>(1, 1);
    matrix[6] = this->extrinsic.at<float>(1, 2);
    matrix[7] = this->extrinsic.at<float>(1, 3);

    matrix[8] = this->extrinsic.at<float>(2, 0);
    matrix[9] = this->extrinsic.at<float>(2, 1);
    matrix[10] = this->extrinsic.at<float>(2, 2);
    matrix[11] = this->extrinsic.at<float>(2, 3);

    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = 0.0;
    matrix[15] = 1.0;

}

void Transformation::computeHomographyMatrix(std::vector<cv::Point2d> &edgeCoordinate) {

    // Input Quadilateral or Image plane coordinates
    Point2f edgeWindow[4];

    // Set the h matrix the same type and size as input
    //h = Mat::zeros(size.height, size.width, CV_32FC1);

    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    edgeWindow[0] = Point2f(0, 0);
    edgeWindow[1] = Point2f(1, 0);
    edgeWindow[2] = Point2f(1, 1);
    edgeWindow[3] = Point2f(0, 1);

    vector<Point2f> in = vector<Point2f>();
    for (const auto &i : edgeWindow) {
        in.push_back(i);
    }

    vector<Point2f> out = vector<Point2f>();
    for(int i = 0; i < 4; i++){
        out.emplace_back((float)edgeCoordinate[i].x / size.width, (float)edgeCoordinate[i].y / size.height);
    }

    this->homography = findHomography(in, out);

    cout << "Homographie\n" << this->homography << endl << endl;
}

void Transformation::computeIntrinsicMatrix() {

    float alpha = 6.5746697944293521e+002f;
    float beta = 6.5746697944293521e+002f;
    float cx = 3.1950000000000000e+002f;
    float cy = 2.3950000000000000e+002f;

    this->intrinsic.at<float>(0, 0) = alpha;
    this->intrinsic.at<float>(0, 1) = 0.0;
    this->intrinsic.at<float>(0, 2) = -cx;

    this->intrinsic.at<float>(1, 0) = 0.0;
    this->intrinsic.at<float>(1, 1) = beta;
    this->intrinsic.at<float>(1, 2) = -cy;

    this->intrinsic.at<float>(2, 0) = 0.0;
    this->intrinsic.at<float>(2, 1) = 0.0;
    this->intrinsic.at<float>(2, 2) = -1.0f;

    cout << "Intrinsic\n" << this->intrinsic << endl << endl;

}

void Transformation::computeExtrinsicMatrix() {

    vector<Mat> rot;
    vector<Mat> trans;
    vector<Mat> norm;
    decomposeHomographyMat(this->homography, this->intrinsic, rot, trans, norm);

    int solution = 0;

    this->extrinsic.at<float>(0, 0) = (float)rot[solution].at<double>(0, 0);
    this->extrinsic.at<float>(0, 1) = (float)rot[solution].at<double>(0, 1);
    this->extrinsic.at<float>(0, 2) = (float)rot[solution].at<double>(0, 2);
    this->extrinsic.at<float>(0, 3) = (float)trans[solution].at<double>(0, 0);

    this->extrinsic.at<float>(1, 0) = (float)rot[solution].at<double>(1, 0);
    this->extrinsic.at<float>(1, 1) = (float)rot[solution].at<double>(1, 1);
    this->extrinsic.at<float>(1, 2) = (float)rot[solution].at<double>(1, 2);
    this->extrinsic.at<float>(1, 3) = (float)trans[solution].at<double>(1, 0);

    this->extrinsic.at<float>(2, 0) = (float)rot[solution].at<double>(2, 0);
    this->extrinsic.at<float>(2, 1) = (float)rot[solution].at<double>(2, 1);
    this->extrinsic.at<float>(2, 2) = (float)rot[solution].at<double>(2, 2);
    this->extrinsic.at<float>(2, 3) = (float)trans[solution].at<double>(2, 0);

    cout << "Extrinsic\n" << this->extrinsic << endl << endl;

}

Mat Transformation::getHomography() {
    return this->homography;
}


