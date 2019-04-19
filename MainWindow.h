#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AngleViewerControllerWidget.h"
#include "AngleViewerWidget.h"

namespace Ui {
	class MainWindow;
}

namespace cv {
	class Mat;
}

class ArduinoController;
class EstereoCameraCapturer;
class Worker;
class QThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

   Worker* m_worker;
   QThread* m_workerThread;
   ArduinoController *m_arduinoController;
	EstereoCameraCapturer *m_estereoCameraCapturer;
   AngleViewerControllerWidget *m_angleViewerControllerWidget1;
   AngleViewerControllerWidget *m_angleViewerControllerWidget2;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	void InitializeStereoCameraViewersQt();
	void InitializeAnglesVisualizers();
	void InitializeWorker();

private:
    Ui::MainWindow *ui;

public slots:
    void ReadArduinoData();
    void ReadArduinoDataAsString();
	void ShowImageCaptured();
	void ShowCannyImageCaptured(const cv::Mat *cvImg1, const cv::Mat *cvImg2);
   void ShowDeepImageCaptured(const cv::Mat *cvImg1, const cv::Mat *cvImg2);
   void SetOpenCVCamerasVisible(bool value);
   void SetAnglesViewerVisible(bool value);

};

#endif // MAINWINDOW_H
