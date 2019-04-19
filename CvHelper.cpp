#include "CvHelper.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
CvHelper::CvHelper()
{

}

void CvHelper::Canny(const cv::Mat &src, cv::Mat &dst, double lowTh, double highTh) {
    cv::Mat imgGrayscale, imgBlurred;
    cv::cvtColor(src, imgGrayscale, cv::COLOR_BGR2GRAY);                   // convert to grayscale
    cv::GaussianBlur(imgGrayscale, imgBlurred,cv::Size(5, 5), 1.8);           // Blur Effect
    cv::Canny(imgBlurred, dst, lowTh, highTh);       // Canny Edge Image
}
