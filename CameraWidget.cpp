#include "CameraWidget.h"
#include "ui_CameraWidget.h"

#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>

#include <QMessageBox>
#include <QPalette>

#include <QtWidgets>

Q_DECLARE_METATYPE(QCameraInfo)

CameraWidget::CameraWidget(QWidget *parent) :
    QWidget (parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);

    connect(ui->devicesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateCameraDevice(int)));
    //Camera devices:
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        ui->devicesComboBox->addItem(cameraInfo.description());
        if (cameraInfo == QCameraInfo::defaultCamera()) {
            ui->devicesComboBox->setCurrentIndex(ui->devicesComboBox->count()-1);
            SetCamera(QCameraInfo::defaultCamera());
        }
    }
}

void CameraWidget::SetCamera(int index)
{
    SetCamera(QCameraInfo::availableCameras().at(index));
}

void CameraWidget::SetCamera(const QCameraInfo &cameraInfo)
{
    m_camera.reset(new QCamera(cameraInfo));

    connect(m_camera.data(), &QCamera::stateChanged, this, &CameraWidget::UpdateCameraState);
    connect(m_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &CameraWidget::DisplayCameraError);

    m_imageCapture.reset(new QCameraImageCapture(m_camera.data()));
    m_camera->setViewfinder(ui->viewfinder);
    UpdateCameraState(m_camera->state());
    UpdateLockStatus(m_camera->lockStatus(), QCamera::UserRequest);

    connect(this->ui->capturePushButton, SIGNAL(clicked()), m_imageCapture.data(), SLOT(capture()));
    connect(m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &CameraWidget::ProcessCapturedImage);
    connect(m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &CameraWidget::imageSaved);
    connect(m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error),
            this, &CameraWidget::DisplayCaptureError);

    connect(m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(&QCamera::lockStatusChanged),
            this, &CameraWidget::UpdateLockStatus);

    m_camera->start();
}

void CameraWidget::keyPressEvent(QKeyEvent * event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_CameraFocus:
        DisplayViewfinder();
        m_camera->searchAndLock();
        event->accept();
        break;
    case Qt::Key_Camera:
        if (m_camera->captureMode() == QCamera::CaptureStillImage) {
            TakeImage();
        } else {

        }
        event->accept();
        break;
    default:
        //QWidget::keyPressEvent(event);
        return;
    }
}

void CameraWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_CameraFocus:
        m_camera->unlock();
        break;
    default:
        //QMainWindow::keyReleaseEvent(event);
        return;
    }
}

void CameraWidget::ProcessCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    img.save("kk.png");
    QImage scaledImage = img.scaled(ui->viewfinder->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display captured image for 4 seconds.
    DisplayCapturedImage();
    QTimer::singleShot(4000, this, &CameraWidget::DisplayViewfinder);
}

void CameraWidget::ToggleLock()
{
    switch (m_camera->lockStatus()) {
    case QCamera::Searching:
    case QCamera::Locked:
        m_camera->unlock();
        break;
    case QCamera::Unlocked:
        m_camera->searchAndLock();
    }
}

void CameraWidget::UpdateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
    QColor indicationColor = Qt::black;

    switch (status) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        ui->statusbar->showMessage(tr("Focusing..."));
        break;
    case QCamera::Locked:
        indicationColor = Qt::darkGreen;
        ui->statusbar->showMessage(tr("Focused"), 2000);
        break;
    case QCamera::Unlocked:
        indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        if (reason == QCamera::LockFailed)
            ui->statusbar->showMessage(tr("Focus Failed"), 2000);
    }
}

void CameraWidget::TakeImage()
{
    m_isCapturingImage = true;
    m_imageCapture->capture();
}

void CameraWidget::DisplayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    //QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    m_isCapturingImage = false;
}

void CameraWidget::StartCamera()
{
    m_camera->start();
}

void CameraWidget::StopCamera()
{
    m_camera->stop();
}

void CameraWidget::UpdateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }
}

void CameraWidget::SetExposureCompensation(int index)
{
    m_camera->exposure()->setExposureCompensation(index*0.5);
}

void CameraWidget::DisplayCameraError()
{
    //QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void CameraWidget::UpdateCameraDevice(int index)
{
    QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();

    if(index >= 0 && index < availableCameras.size()) {
        SetCamera(availableCameras.takeAt(index));
    }

}

void CameraWidget::DisplayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void CameraWidget::DisplayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void CameraWidget::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);

    m_isCapturingImage = false;
    if (m_applicationExiting) {
        //close();
    }
}

void CameraWidget::closeEvent(QCloseEvent *event)
{
    if (m_isCapturingImage) {
        //setEnabled(false);
        m_applicationExiting = true;
        event->ignore();
    } else {
        event->accept();
    }
}
