#ifndef CAMERACAPTURER_H
#define CAMERACAPTURER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <thread>
#include <mutex>

class CameraCapturer : public QObject
{
    Q_OBJECT
    cv::VideoCapture m_capture;
    cv::Mat m_image;
    cv::Mat *m_imageCopy;
    QImage *m_qImage;
    bool m_isCameraRunning;
    QTimer *m_cameraTimer;
    int m_index;
    int m_width;
    int m_height;
public:
    CameraCapturer(int m_index, int m_width, int m_height, QTimer *timer = 0);
    ~CameraCapturer();

public slots:
    void Start();
    void Stop();
    void CameraTimerTimeout();
    void GetCapture(QImage **qImage) const;
    void GetCapture(cv::Mat **cvImage) const;

signals:
    void imageCaptured();
};

#endif // CAMERACAPTURER_H
