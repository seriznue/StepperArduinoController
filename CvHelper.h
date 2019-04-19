#ifndef CVHELPER_H
#define CVHELPER_H

namespace cv {
class Mat;
}

class CvHelper
{
public:
    CvHelper();

    static void Canny(const cv::Mat &src, cv::Mat &dst, double lowTh = 10 , double highTh = 100);
};

#endif // CVHELPER_H
