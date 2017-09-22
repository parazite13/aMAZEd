#ifndef AMAZED_TRANSFORMATION_H
#define AMAZED_TRANSFORMATION_H

#include <opencv2/core/core.hpp>

class Transformation {

public:
    static void getHomographyMatrix(float matrix[], const cv::Point2f inputQuad[], const cv::Size size);


};


#endif //AMAZED_TRANSFORMATION_H
