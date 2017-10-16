#include "AngleModel.h"

using namespace cv;

AngleModel::AngleModel(Transformation &initialTransformation) {

    this->initialTransformation = &initialTransformation;
    Mat homography = initialTransformation.getHomography();

    /// Vect X
    Mat vectX = Mat(3, 1, CV_64FC1);
    vectX.at<double>(0) = VECT_LENGTH;
    vectX.at<double>(1) = 0;
    vectX.at<double>(2) = 0;
    this->initialVectX = homography * vectX;

    /// Vect Y
    Mat vectY = Mat(3, 1, CV_64FC1);
    vectY.at<double>(0) = 0;
    vectY.at<double>(1) = VECT_LENGTH;
    vectY.at<double>(2) = 0;
    this->initialVectY = homography * vectY;

    /// Vect Z
    Mat vectZ = Mat(3, 1, CV_64FC1);
    vectZ.at<double>(0) = 0;
    vectZ.at<double>(1) = 0;
    vectZ.at<double>(2) = VECT_LENGTH;
    this->initialVectZ = homography * vectZ;
}

AngleModel::AngleModel(Transformation &initialTransformation, Transformation &currentTransformation) {
    this->initialTransformation = &initialTransformation;
    this->currentTransformation = &currentTransformation;
}

void AngleModel::setCurrentTransformation(Transformation &currentTransformation) {
    this->currentTransformation = &currentTransformation;
    computeAngle();
}

double AngleModel::scalarProduct(Mat const &a, Mat const &b) const{
    return  a.at<double>(0) * b.at<double>(0) +
            a.at<double>(1) * b.at<double>(1) +
            a.at<double>(2) * b.at<double>(2);
}

double AngleModel::crossProduct(cv::Mat const &a, cv::Mat const &b) const {
    return a.at<double>(1) * b.at<double>(2) - a.at<double>(2) * b.at<double>(1) +
           b.at<double>(0) * a.at<double>(2) - a.at<double>(0) * b.at<double>(2) +
           a.at<double>(0) * b.at<double>(1) - a.at<double>(1) * b.at<double>(0);
}

double AngleModel::norm(cv::Mat const &a) const{
    return sqrt(
            a.at<double>(0) * a.at<double>(0) +
            a.at<double>(1) * a.at<double>(1) +
            a.at<double>(2) * a.at<double>(2)
    );
}

double AngleModel::angleOriente(cv::Mat &a, cv::Mat &b) const {

    double normal = norm(a) * norm(b);

    double sinus = crossProduct(a, b) / normal;
    double cosinus = scalarProduct(a, b) / normal;

    if(sinus > 0){
        return acos(cosinus);
    }else{
        return -acos(cosinus);
    }

}

void AngleModel::computeAngle() {

    Mat homography = currentTransformation->getHomography();

    /// Angle X
    Mat vectX = Mat(3, 1, CV_64FC1);
    vectX.at<double>(0) = VECT_LENGTH;
    vectX.at<double>(1) = 0;
    vectX.at<double>(2) = 0;
    Mat currentVectX = homography * vectX;
    this->angleX = angleOriente(initialVectX, currentVectX);

    /// Angle Y
    Mat vectY = Mat(3, 1, CV_64FC1);
    vectY.at<double>(0) = 0;
    vectY.at<double>(1) = VECT_LENGTH;
    vectY.at<double>(2) = 0;
    Mat currentVectY = homography * vectY;
    this->angleY = angleOriente(initialVectY, currentVectY);

    /// Angle Z
    Mat vectZ = Mat(3, 1, CV_64FC1);
    vectZ.at<double>(0) = 0;
    vectZ.at<double>(1) = 0;
    vectZ.at<double>(2) = VECT_LENGTH;
    Mat currentVectZ = homography * vectZ;
    this->angleZ = angleOriente(initialVectZ, currentVectZ);

}

double AngleModel::getAngleX() const {
    return angleX;
}

double AngleModel::getAngleY() const {
    return angleY;
}

double AngleModel::getAngleZ() const {
    return angleZ;
}
