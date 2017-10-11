#ifndef AMAZED_TRANSFORMATION_H
#define AMAZED_TRANSFORMATION_H

#include <opencv2/core/core.hpp>

class Transformation {

private:

    const float FX = 550;
    const float FY = 550;
    double X0;
    double Y0;

    cv::Size size;  // size.width => w, size.height => h
    double N;
    double F;

    cv::Mat K;  // Intrinsic
    cv::Mat P;  // Extrinsic
    cv::Mat H;  // Homography
    cv::Mat NDC;  // NDC
    cv::Mat Persp;  // Persp

    cv::Mat proj;
    cv::Mat modelView;

    void computeHomographyMatrix(const std::vector<cv::Point2d> &edgeCoordinate);
    void computeIntrinsicMatrix();
    void computeExtrinsicMatrix(const std::vector<cv::Point2d> &edgeCoordinate);
    void computeProjMatrix();
    void computeModelviewMatrix(const std::vector<cv::Point2d> &edgeCoordinate);
    void computeNDCMatrix();
    void computePerspMatrix();

public:

    Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, double near, double far);

    void getModelviewMatrix(double matrix[]);
    void getProjectionMatrix(double matrix[]);

    void getHomography(double matrix[]);
};


#endif //AMAZED_TRANSFORMATION_H
