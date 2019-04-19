#include "EstereoCameraCapturer.h"
#include <QImage>
#include <stdio.h>
#include <iostream>
//#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <QDebug>

using namespace cv;

EstereoCameraCapturer::EstereoCameraCapturer(int width, int height, QTimer *timer) :
    QObject(),
    m_cameraTimer(0),
    m_isCameraRunning (false),
    m_width(width),
    m_height(height)
{
    if(timer)
        m_cameraTimer = timer;

}

void EstereoCameraCapturer::Start()
{
    if(!m_isCameraRunning)
    {
        // open camera stream
        m_capture1.open(0); // default: 0
        m_capture2.open(1); // default: 0

        // set the acquired frame size to the size of its container
        m_capture1.set(CAP_PROP_FRAME_WIDTH, m_width);
        m_capture1.set(CAP_PROP_FRAME_HEIGHT, m_height);
        m_capture2.set(CAP_PROP_FRAME_WIDTH, m_width);
        m_capture2.set(CAP_PROP_FRAME_HEIGHT, m_height);

        m_isCameraRunning = true;
        connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
    }
}

void EstereoCameraCapturer::Stop()
{
    if(!m_capture1.isOpened() || !m_capture2.isOpened())
        return;

    // stop timer
    m_cameraTimer->stop();
    // release camera stream
    m_capture1.release();
    m_capture2.release();

    m_isCameraRunning = false;

    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

void EstereoCameraCapturer::CameraTimerTimeout()
{
    if(!m_isCameraRunning) return;

    if(m_capture1.isOpened()) m_capture1 >> m_image1;
    if(m_capture2.isOpened()) m_capture2 >> m_image2;

    emit imageCaptured();
}

void EstereoCameraCapturer::GetCapture(cv::Mat &cvImage1, cv::Mat &cvImage2) const {
    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));

    if(m_capture1.isOpened())
        cvImage1 = (m_image1.clone());
    else
        cvImage1 = Mat(m_height, m_width, CV_8UC3, Scalar(0,0,0));

    if(m_capture2.isOpened())
        cvImage2 = (m_image2.clone());
    else
        cvImage2 = Mat(m_height, m_width, CV_8UC3, Scalar(0,0,0));

    connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

EstereoCameraCapturer::~EstereoCameraCapturer()
{
}
