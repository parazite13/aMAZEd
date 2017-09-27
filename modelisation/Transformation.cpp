#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include "Transformation.h"

using namespace cv;
using namespace std;

Transformation::Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, float near, float far) {

    this->X0 = size.width / 2;
    this->Y0 = size.height / 2;

    this->size = size;
    this->near = near;
    this->far = far;

    computeHomographyMatrix(edgeCoordinate);

    this->K = Mat(3, 3, CV_32FC1);
    computeIntrinsicMatrix();

    this->modelView = Mat(4, 4, CV_32FC1);
    computeModelviewMatrix();

    this->proj = Mat(4, 4, CV_32FC1);
    computeProjMatrix();

}


void Transformation::getProjectionMatrix(float *matrix) {

    matrix[0] = this->proj.at<float>(0, 0);
    matrix[1] = this->proj.at<float>(0, 1);
    matrix[2] = this->proj.at<float>(0, 2);
    matrix[3] = this->proj.at<float>(0, 3);

    matrix[4] = this->proj.at<float>(1, 0);
    matrix[5] = this->proj.at<float>(1, 1);
    matrix[6] = this->proj.at<float>(1, 2);
    matrix[7] = this->proj.at<float>(1, 3);

    matrix[8] = this->proj.at<float>(2, 0);
    matrix[9] = this->proj.at<float>(2, 1);
    matrix[10] = this->proj.at<float>(2, 2);
    matrix[11] = this->proj.at<float>(2, 3);

    matrix[12] = this->proj.at<float>(3, 0);
    matrix[13] = this->proj.at<float>(3, 1);
    matrix[14] = this->proj.at<float>(3, 2);
    matrix[15] = this->proj.at<float>(3, 3);

}

void Transformation::getModelviewMatrix(float *matrix) {

    matrix[0] = this->modelView.at<float>(0, 0);
    matrix[1] = this->modelView.at<float>(0, 1);
    matrix[2] = this->modelView.at<float>(0, 2);
    matrix[3] = this->modelView.at<float>(0, 3);

    matrix[4] = this->modelView.at<float>(1, 0);
    matrix[5] = this->modelView.at<float>(1, 1);
    matrix[6] = this->modelView.at<float>(1, 2);
    matrix[7] = this->modelView.at<float>(1, 3);

    matrix[8] = this->modelView.at<float>(2, 0);
    matrix[9] = this->modelView.at<float>(2, 1);
    matrix[10] = this->modelView.at<float>(2, 2);
    matrix[11] = this->modelView.at<float>(2, 3);

    matrix[12] = this->modelView.at<float>(3, 0);
    matrix[13] = this->modelView.at<float>(3, 1);
    matrix[14] = this->modelView.at<float>(3, 2);
    matrix[15] = this->modelView.at<float>(3, 3);

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

    cout << "Homographie\n" << this->H << endl << endl;
}

void Transformation::computeIntrinsicMatrix() {

    this->K.at<float>(0, 0) = this->FX;
    this->K.at<float>(0, 1) = 0.0f;
    this->K.at<float>(0, 2) = -this->X0;

    this->K.at<float>(1, 0) = 0.0f;
    this->K.at<float>(1, 1) = this->FY;
    this->K.at<float>(1, 2) = -this->Y0;

    this->K.at<float>(2, 0) = 0.0f;
    this->K.at<float>(2, 1) = 0.0f;
    this->K.at<float>(2, 2) = -1.0f;

    cout << "Intrinsic\n" << this->K << endl << endl;

}

void Transformation::computeExtrinsicMatrix() {

    this->P = Mat(3, 4, CV_32FC1);

    vector<Mat> rot;
    vector<Mat> trans;
    vector<Mat> norm;
    decomposeHomographyMat(this->H, this->K, rot, trans, norm);

    int solution = 0;

    this->P.at<float>(0, 0) = (float)rot[solution].at<double>(0, 0);
    this->P.at<float>(0, 1) = (float)rot[solution].at<double>(0, 1);
    this->P.at<float>(0, 2) = -(float)rot[solution].at<double>(0, 2);
    this->P.at<float>(0, 3) = (float)trans[solution].at<double>(0, 0);

    this->P.at<float>(1, 0) = (float)rot[solution].at<double>(1, 0);
    this->P.at<float>(1, 1) = (float)rot[solution].at<double>(1, 1);
    this->P.at<float>(1, 2) = -(float)rot[solution].at<double>(1, 2);
    this->P.at<float>(1, 3) = (float)trans[solution].at<double>(1, 0);

    this->P.at<float>(2, 0) = (float)rot[solution].at<double>(2, 0);
    this->P.at<float>(2, 1) = (float)rot[solution].at<double>(2, 1);
    this->P.at<float>(2, 2) = -(float)rot[solution].at<double>(2, 2);
    this->P.at<float>(2, 3) = (float)trans[solution].at<double>(2, 0);

    /// Si le déterminant de Rot vaut -1 on change le signe de la matrice
    if(fabs(determinant(rot[solution]) + 1) < 1e-3){
        rot[solution] *= -1;
    }

    cout << "det R = " << determinant(rot[solution]) << endl;

    cout << "Extrinsic\n" << this->P << endl << endl;

}

void Transformation::computeProjMatrix() {

    computeNDCMatrix();
    computePerspMatrix();

    this->proj = this->NDC * this->Persp;

    cout << "proj\n" << this->proj << endl << endl;
}

void Transformation::computeModelviewMatrix() {

    computeExtrinsicMatrix();

    this->modelView.at<float>(0, 0) = this->P.at<float>(0, 0);
    this->modelView.at<float>(0, 1) = this->P.at<float>(0, 1);
    this->modelView.at<float>(0, 2) = this->P.at<float>(0, 2);
    this->modelView.at<float>(0, 3) = this->P.at<float>(0, 3);

    this->modelView.at<float>(1, 0) = this->P.at<float>(1, 0);
    this->modelView.at<float>(1, 1) = this->P.at<float>(1, 1);
    this->modelView.at<float>(1, 2) = this->P.at<float>(1, 2);
    this->modelView.at<float>(1, 3) = this->P.at<float>(1, 3);

    this->modelView.at<float>(2, 0) = this->P.at<float>(2, 0);
    this->modelView.at<float>(2, 1) = this->P.at<float>(2, 1);
    this->modelView.at<float>(2, 2) = this->P.at<float>(2, 2);
    this->modelView.at<float>(2, 3) = this->P.at<float>(2, 3);

    this->modelView.at<float>(3, 0) = 0.0f;
    this->modelView.at<float>(3, 1) = 0.0f;
    this->modelView.at<float>(3, 2) = 0.0f;
    this->modelView.at<float>(3, 3) = 1.0f;

    cout << "modelView\n" << this->modelView << endl << endl;

}

void Transformation::computeNDCMatrix() {

    this->NDC = Mat(4, 4, CV_32FC1);

    float left = 0.0f;
    float right = this->size.width;
    float bottom = 0.0f;
    float top = this->size.height;

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(this->far + this->near) / (this->far - this->near);

    this->NDC.at<float>(0, 0) = 2.0f / (right - left);
    this->NDC.at<float>(0, 1) = 0.0f;
    this->NDC.at<float>(0, 2) = 0.0f;
    this->NDC.at<float>(0, 3) = tx;

    this->NDC.at<float>(1, 0) = 0.0f;
    this->NDC.at<float>(1, 1) = 2.0f / (top - bottom);
    this->NDC.at<float>(1, 2) = 0.0f;
    this->NDC.at<float>(1, 3) = ty;

    this->NDC.at<float>(2, 0) = 0.0f;
    this->NDC.at<float>(2, 1) = 0.0f;
    this->NDC.at<float>(2, 2) = 2.0f / (this->near - this->far);
    this->NDC.at<float>(2, 3) = tz;

    this->NDC.at<float>(3, 0) = 0.0f;
    this->NDC.at<float>(3, 1) = 0.0f;
    this->NDC.at<float>(3, 2) = 0.0f;
    this->NDC.at<float>(3, 3) = 1.0f;

    cout << "NDC\n" << this->NDC << endl << endl;

}

void Transformation::computePerspMatrix() {

    this->Persp = Mat(4, 4, CV_32FC1);

    this->Persp.at<float>(0, 0) = this->K.at<float>(0, 0);
    this->Persp.at<float>(0, 1) = this->K.at<float>(0, 1);
    this->Persp.at<float>(0, 2) = this->K.at<float>(0, 2);
    this->Persp.at<float>(0, 3) = 0.0f;

    this->Persp.at<float>(1, 0) = this->K.at<float>(1, 0);
    this->Persp.at<float>(1, 1) = this->K.at<float>(1, 1);
    this->Persp.at<float>(1, 2) = this->K.at<float>(1, 2);
    this->Persp.at<float>(1, 3) = 0.0f;

    this->Persp.at<float>(2, 0) = this->K.at<float>(2, 0);
    this->Persp.at<float>(2, 1) = this->K.at<float>(2, 1);
    this->Persp.at<float>(2, 2) = this->near + this->far;
    this->Persp.at<float>(2, 3) = this->near * this->far;

    this->Persp.at<float>(3, 0) = 0.0f;
    this->Persp.at<float>(3, 1) = 0.0f;
    this->Persp.at<float>(3, 2) = -1.0f;
    this->Persp.at<float>(3, 3) = 0.0f;

    cout << "Persp\n" << this->Persp << endl << endl;

}

