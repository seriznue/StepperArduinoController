#ifndef ESTEREOCAMERACAPTURER_H
#define ESTEREOCAMERACAPTURER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <thread>
#include <mutex>

class EstereoCameraCapturer : public QObject
{
    Q_OBJECT
    cv::VideoCapture m_capture1;
    cv::VideoCapture m_capture2;
    cv::Mat m_image1;
    cv::Mat m_image2;
    bool m_isCameraRunning;
    QTimer *m_cameraTimer;
    int m_width;
    int m_height;
public:
    EstereoCameraCapturer(int m_width, int m_height, QTimer *timer = 0);
    ~EstereoCameraCapturer();

public slots:
    void Start();
    void Stop();
    void CameraTimerTimeout();
    void GetCapture(cv::Mat &cvImage1, cv::Mat &cvImage2) const;

signals:
    void imageCaptured();
};

#endif // ESTEREOCAMERACAPTURER_H
