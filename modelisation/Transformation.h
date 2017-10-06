#ifndef AMAZED_TRANSFORMATION_H
#define AMAZED_TRANSFORMATION_H

#include <opencv2/core/core.hpp>

class Transformation {

private:

    const float FX = 550;
    const float FY = 550;
    float X0;
    float Y0;

    cv::Size size;  // size.width => w, size.height => h
    float N;
    float F;

    cv::Mat K;  // Intrinsic
    cv::Mat P;  // Extrinsic
    cv::Mat H;  // Homography
    cv::Mat NDC;  // NDC
    cv::Mat Persp;  // Persp

    cv::Mat proj;
    cv::Mat modelView;

    void computeHomographyMatrix(std::vector<cv::Point2d> &edgeCoordinate);
    void computeIntrinsicMatrix();
    void computeExtrinsicMatrix();
    void computeProjMatrix();
    void computeModelviewMatrix();
    void computeNDCMatrix();
    void computePerspMatrix();

public:

    Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, float near, float far);

    void getModelviewMatrix(float matrix[]);
    void getProjectionMatrix(float matrix[]);

    void getHomography(float matrix[]);
};


#endif //AMAZED_TRANSFORMATION_H
