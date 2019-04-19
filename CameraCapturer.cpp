#include "CameraCapturer.h"
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

CameraCapturer::CameraCapturer(int index, int width, int height, QTimer *timer) :
    QObject(),
    m_cameraTimer(0),
    m_isCameraRunning (false),
    m_index(index),
    m_width(width),
    m_height(height),
    m_qImage(0)
{
    if(timer)
        m_cameraTimer = timer;

}

void CameraCapturer::Start()
{
    if(!m_isCameraRunning)
    {
        // open camera stream
        m_capture.open(m_index, CAP_ANY); // default: 0

        if(!m_capture.isOpened())
            return;

        // set the acquired frame size to the size of its container
        m_capture.set(cv::CAP_PROP_FRAME_WIDTH, m_width);
        m_capture.set(CAP_PROP_FRAME_HEIGHT, m_height);

        m_isCameraRunning = true;

        // start timer for acquiring the video
        //m_cameraTimer->start(33); // 33 ms = 30 fps
        // at the timeout() event, execute the cameraTimerTimeout() method
        connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
    }

}

void CameraCapturer::Stop()
{
    if(!m_capture.isOpened())
        return;

    // stop timer
    m_cameraTimer->stop();
    // release camera stream
    m_capture.release();

    m_isCameraRunning = false;

    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

void CameraCapturer::CameraTimerTimeout()
{
    if(m_isCameraRunning && m_capture.isOpened())
    {
        m_capture >> m_image;

        if(m_qImage) {
            delete m_qImage;
            m_qImage = 0;
        }

        m_qImage = new QImage((const unsigned char*)(m_image.data), m_image.cols, m_image.rows, m_image.step, QImage::Format_RGB888);

        emit imageCaptured();
    }

}

void CameraCapturer::GetCapture(QImage **qImage) const {
    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));

    if (m_qImage) {
        *qImage = new QImage(*m_qImage);
    }

    connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

void CameraCapturer::GetCapture(cv::Mat **cvImage) const {
    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));

    if (m_qImage) {
        *cvImage = new cv::Mat(m_image.clone());
    }

    connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

CameraCapturer::~CameraCapturer()
{
    if(m_qImage) {
        delete m_qImage;
        m_qImage = 0;
    }
    if(m_cameraTimer) {
        delete m_cameraTimer;
        m_cameraTimer = 0;
    }
}
