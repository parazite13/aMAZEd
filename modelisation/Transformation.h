#ifndef AMAZED_TRANSFORMATION_H
#define AMAZED_TRANSFORMATION_H

#include <opencv2/core/core.hpp>

class Transformation {

private:

    cv::Size size;
    float near;
    float far;

    cv::Mat intrinsic;
    cv::Mat extrinsic;
    cv::Mat homography;

    void computeHomographyMatrix(std::vector<cv::Point2d> &edgeCoordinate);
    void computeIntrinsicMatrix();
    void computeExtrinsicMatrix();

public:

    Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size, float near, float far);

    void getModelviewMatrix(float matrix[]);
    void getProjectionMatrix(float matrix[]);
    cv::Mat getHomography();

};


#endif //AMAZED_TRANSFORMATION_H
