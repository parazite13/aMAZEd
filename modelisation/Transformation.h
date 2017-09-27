#ifndef AMAZED_TRANSFORMATION_H
#define AMAZED_TRANSFORMATION_H

#include <opencv2/core/core.hpp>

class Transformation {

private:

    cv::Size size;  // size.width => w, size.height => h
    cv::Mat H;  // Homography


    void computeHomographyMatrix(std::vector<cv::Point2d> &edgeCoordinate);


public:

    Transformation(std::vector<cv::Point2d> &edgeCoordinate, cv::Size size);

    const void getHomography(float* matrix) const;

};


#endif //AMAZED_TRANSFORMATION_H
