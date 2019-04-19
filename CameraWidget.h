#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class CameraWidget;
}
QT_END_NAMESPACE

class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    CameraWidget(QWidget *parent = nullptr);

    void SetCamera(int index);

private slots:
    void SetCamera(const QCameraInfo &cameraInfo);

    void StartCamera();
    void StopCamera();

    void ToggleLock();
    void TakeImage();
    void DisplayCaptureError(int, QCameraImageCapture::Error, const QString &errorString);

    void DisplayCameraError();

    void UpdateCameraDevice(int index);

    void UpdateCameraState(QCamera::State);
    void SetExposureCompensation(int index);

    void ProcessCapturedImage(int requestId, const QImage &img);
    void UpdateLockStatus(QCamera::LockStatus, QCamera::LockChangeReason);

    void DisplayViewfinder();
    void DisplayCapturedImage();

    void imageSaved(int id, const QString &fileName);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::CameraWidget *ui;

    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QCameraImageCapture> m_imageCapture;

    QImageEncoderSettings m_imageSettings;
    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
};

#endif // CAMERAWIDGET_H
