#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include "Transformation.h"

using namespace cv;
using namespace std;

Transformation::Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, double near, double far) {

    this->X0 = size.width / 2;
    this->Y0 = size.height / 2;

    this->size = size;
    this->N = near;
    this->F = far;

    computeHomographyMatrix(edgeCoordinate);
    computeIntrinsicMatrix();
    computeModelviewMatrix(edgeCoordinate);
    computeProjMatrix();

}

void Transformation::getHomography(double *matrix) {

    matrix[0] = this->H.at<double>(0, 0);
    matrix[1] = this->H.at<double>(1, 0);
    matrix[2] = 0.0f;
    matrix[3] = this->H.at<double>(2, 0);

    matrix[4] = this->H.at<double>(0, 1);
    matrix[5] = this->H.at<double>(1, 1);
    matrix[6] = 0.0f;
    matrix[7] = this->H.at<double>(2, 1);

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = this->H.at<double>(0, 2);
    matrix[13] = this->H.at<double>(1, 2);
    matrix[14] = 0.0f;
    matrix[15] = this->H.at<double>(2, 2);
}


void Transformation::getProjectionMatrix(double *matrix) {

    matrix[0] = this->proj.at<double>(0, 0);
    matrix[1] = this->proj.at<double>(1, 0);
    matrix[2] = this->proj.at<double>(2, 0);
    matrix[3] = this->proj.at<double>(3, 0);

    matrix[4] = this->proj.at<double>(0, 1);
    matrix[5] = this->proj.at<double>(1, 1);
    matrix[6] = this->proj.at<double>(2, 1);
    matrix[7] = this->proj.at<double>(3, 1);

    matrix[8] = this->proj.at<double>(0, 2);
    matrix[9] = this->proj.at<double>(1, 2);
    matrix[10] = this->proj.at<double>(2, 2);
    matrix[11] = this->proj.at<double>(3, 2);

    matrix[12] = this->proj.at<double>(0, 3);
    matrix[13] = this->proj.at<double>(1, 3);
    matrix[14] = this->proj.at<double>(2, 3);
    matrix[15] = this->proj.at<double>(3, 3);

}

void Transformation::getModelviewMatrix(double *matrix) {

    matrix[0] = this->modelView.at<double>(0, 0);
    matrix[1] = this->modelView.at<double>(1, 0);
    matrix[2] = this->modelView.at<double>(2, 0);
    matrix[3] = this->modelView.at<double>(3, 0);

    matrix[4] = this->modelView.at<double>(0, 1);
    matrix[5] = this->modelView.at<double>(1, 1);
    matrix[6] = this->modelView.at<double>(2, 1);
    matrix[7] = this->modelView.at<double>(3, 1);

    matrix[8] = this->modelView.at<double>(0, 2);
    matrix[9] = this->modelView.at<double>(1, 2);
    matrix[10] = this->modelView.at<double>(2, 2);
    matrix[11] = this->modelView.at<double>(3, 2);

    matrix[12] = this->modelView.at<double>(0, 3);
    matrix[13] = this->modelView.at<double>(1, 3);
    matrix[14] = this->modelView.at<double>(2, 3);
    matrix[15] = this->modelView.at<double>(3, 3);

}

void Transformation::computeHomographyMatrix(const vector<Point2d> &edgeCoordinate) {

    this->proj = Mat(4, 4, CV_64FC1);

    vector<Point2f> imagePoints;
    for(const auto &i : edgeCoordinate){
        imagePoints.emplace_back(i.x, this->size.height - i.y);
    }

    vector<Point2d> modelPoints;
    modelPoints.emplace_back(0.0, 0.0);
    modelPoints.emplace_back(0.0, 1.0);
    modelPoints.emplace_back(1.0, 1.0);
    modelPoints.emplace_back(1.0, 0.0);

    this->H = findHomography(modelPoints, imagePoints, RANSAC);
}

void Transformation::computeIntrinsicMatrix() {

    this->K = Mat(3, 3, CV_64FC1);

    this->K.at<double>(0, 0) = this->FX;
    this->K.at<double>(0, 1) = 0.0f;
    this->K.at<double>(0, 2) = this->X0;

    this->K.at<double>(1, 0) = 0.0f;
    this->K.at<double>(1, 1) = this->FY;
    this->K.at<double>(1, 2) = this->Y0;

    this->K.at<double>(2, 0) = 0.0f;
    this->K.at<double>(2, 1) = 0.0f;
    this->K.at<double>(2, 2) = 1.0f;

}

void Transformation::computeExtrinsicMatrix(const vector<Point2d> &edgeCoordinate) {

    vector<Point2f> imagePoints;
    for(const auto &i : edgeCoordinate){
        imagePoints.emplace_back(i.x, this->size.height - i.y); /// Inversion du repere selon l'axe Y
    }

    vector<Point3d> modelPoints;
    modelPoints.emplace_back(0.0, 0.0, 0.0);
    modelPoints.emplace_back(0.0, 1.0, 0.0);
    modelPoints.emplace_back(1.0, 1.0, 0.0);
    modelPoints.emplace_back(1.0, 0.0, 0.0);

    Mat distCoeffs = Mat::zeros(4, 1, CV_64FC1); // Pas de distortion (très grossier -> pas de calibrage)

    Mat rot;
    Mat trans;

    solvePnP(modelPoints, imagePoints, this->K, distCoeffs, rot, trans);

    /// Affichae OpenCV fonctionne uniquement en enlevant la premiere boucle d'inversion du y
    /*vector<Point3d> modelPoints3d;
    vector<Point2d> projectedPoints;
    for(auto p : modelPoints)
        modelPoints3d.push_back(Point3d(p.x, p.y, 1.));

    projectPoints(modelPoints3d, rot, trans, this->K, distCoeffs, projectedPoints);

    for(int i = 0; i < imagePoints.size(); i++) {
        circle(currentFrame, imagePoints[i], 3, Scalar(0,0,255), -1);
    }

    int i = 0;
    for(auto p:imagePoints)
        cv::line(currentFrame,p, projectedPoints[i++], cv::Scalar(255,0,0), 2);

    cv::imshow("ReVA 2017", currentFrame);
*/

    Mat rotRodrigues;
    Rodrigues(rot, rotRodrigues);

    cout << "ROT" << endl << rotRodrigues << endl;
    cout << "TRANS" << endl << trans << endl;


    this->P = Mat(3, 4, CV_64FC1);

    this->P.at<double>(0, 0) = rotRodrigues.at<double>(0, 0);
    this->P.at<double>(0, 1) = rotRodrigues.at<double>(0, 1);
    this->P.at<double>(0, 2) = rotRodrigues.at<double>(0, 2);
    this->P.at<double>(0, 3) = trans.at<double>(0);

    this->P.at<double>(1, 0) = rotRodrigues.at<double>(1, 0);
    this->P.at<double>(1, 1) = rotRodrigues.at<double>(1, 1);
    this->P.at<double>(1, 2) = rotRodrigues.at<double>(1, 2);
    this->P.at<double>(1, 3) = trans.at<double>(1);

    this->P.at<double>(2, 0) = rotRodrigues.at<double>(2, 0);
    this->P.at<double>(2, 1) = rotRodrigues.at<double>(2, 1);
    this->P.at<double>(2, 2) = rotRodrigues.at<double>(2, 2);
    this->P.at<double>(2, 3) = trans.at<double>(2);

}

void Transformation::computeProjMatrix() {

    computeNDCMatrix();
    computePerspMatrix();

    this->proj = this->NDC *this->Persp;
}

void Transformation::computeModelviewMatrix(const vector<Point2d> &edgeCoordinate) {

    this->modelView = Mat(4, 4, CV_64FC1);

    computeExtrinsicMatrix(edgeCoordinate);

    this->modelView.at<double>(0, 0) = this->P.at<double>(0, 0);
    this->modelView.at<double>(0, 1) = this->P.at<double>(0, 1);
    this->modelView.at<double>(0, 2) = this->P.at<double>(0, 2);
    this->modelView.at<double>(0, 3) = this->P.at<double>(0, 3);

    this->modelView.at<double>(1, 0) = this->P.at<double>(1, 0);
    this->modelView.at<double>(1, 1) = this->P.at<double>(1, 1);
    this->modelView.at<double>(1, 2) = this->P.at<double>(1, 2);
    this->modelView.at<double>(1, 3) = this->P.at<double>(1, 3);

    this->modelView.at<double>(2, 0) = this->P.at<double>(2, 0);
    this->modelView.at<double>(2, 1) = this->P.at<double>(2, 1);
    this->modelView.at<double>(2, 2) = this->P.at<double>(2, 2);
    this->modelView.at<double>(2, 3) = this->P.at<double>(2, 3);

    this->modelView.at<double>(3, 0) = 0.0f;
    this->modelView.at<double>(3, 1) = 0.0f;
    this->modelView.at<double>(3, 2) = 0.0f;
    this->modelView.at<double>(3, 3) = 1.0f;

}

void Transformation::computeNDCMatrix() {

    this->NDC = Mat(4, 4, CV_64FC1);

    double left = 0.0f;
    double right = this->size.width;
    double bottom = 0.0f;
    double top = this->size.height;

    double tx = -(right + left) / (right - left);
    double ty = -(top + bottom) / (top - bottom);
    double tz = -(this->F + this->N) / (this->F - this->N);

    this->NDC.at<double>(0, 0) = 2.0f / (right - left);
    this->NDC.at<double>(0, 1) = 0.0f;
    this->NDC.at<double>(0, 2) = 0.0f;
    this->NDC.at<double>(0, 3) = tx;

    this->NDC.at<double>(1, 0) = 0.0f;
    this->NDC.at<double>(1, 1) = 2.0f / (top - bottom);
    this->NDC.at<double>(1, 2) = 0.0f;
    this->NDC.at<double>(1, 3) = ty;

    this->NDC.at<double>(2, 0) = 0.0f;
    this->NDC.at<double>(2, 1) = 0.0f;
    this->NDC.at<double>(2, 2) = 2.0f / (this->F - this->N);
    this->NDC.at<double>(2, 3) = tz;

    this->NDC.at<double>(3, 0) = 0.0f;
    this->NDC.at<double>(3, 1) = 0.0f;
    this->NDC.at<double>(3, 2) = 0.0f;
    this->NDC.at<double>(3, 3) = 1.0f;

}

void Transformation::computePerspMatrix() {

    this->Persp = Mat(4, 4, CV_64FC1);

    this->Persp.at<double>(0, 0) = this->K.at<double>(0, 0);
    this->Persp.at<double>(0, 1) = this->K.at<double>(0, 1);
    this->Persp.at<double>(0, 2) = this->K.at<double>(0, 2);
    this->Persp.at<double>(0, 3) = 0.0f;

    this->Persp.at<double>(1, 0) = 0.0f;
    this->Persp.at<double>(1, 1) = this->K.at<double>(1, 1);
    this->Persp.at<double>(1, 2) = this->K.at<double>(1, 2);
    this->Persp.at<double>(1, 3) = 0.0f;

    this->Persp.at<double>(2, 0) = 0.0f;
    this->Persp.at<double>(2, 1) = 0.0f;
    this->Persp.at<double>(2, 2) = this->N + this->F;
    this->Persp.at<double>(2, 3) = -this->N * this->F;  /// LE MOINS DOU IL SORT ?!?!????

    this->Persp.at<double>(3, 0) = 0.0f;
    this->Persp.at<double>(3, 1) = 0.0f;
    this->Persp.at<double>(3, 2) = this->K.at<double>(2, 2);
    this->Persp.at<double>(3, 3) = 0.0f;

}