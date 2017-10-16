#ifndef AMAZED_ANGLE_H
#define AMAZED_ANGLE_H


#include "../modelisation/Transformation.h"

class AngleModel {

private:

    const int VECT_LENGTH = 500;

    Transformation* initialTransformation;
    Transformation* currentTransformation;

    cv::Mat initialVectX;
    cv::Mat initialVectY;
    cv::Mat initialVectZ;

    double angleX;
    double angleY;
    double angleZ;

    double scalarProduct(cv::Mat const &a, cv::Mat const &b) const;
    double crossProduct(cv::Mat const &a, cv::Mat const &b) const;
    double norm(cv::Mat const &a)const ;
    void computeAngle();
    double angleOriente(cv::Mat &a, cv::Mat &b) const;

public:

    void setCurrentTransformation(Transformation &currentTransformation);

    AngleModel(Transformation &initialTransformation);
    AngleModel(Transformation &initialTransformation, Transformation &currentTransformation);

    double getAngleX() const;
    double getAngleY() const;
    double getAngleZ() const;

};


#endif //AMAZED_ANGLE_H
