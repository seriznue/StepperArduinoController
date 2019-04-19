#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ArduinoController.h"
#include "AngleViewerControllerWidget.h"
#include "CameraWidget.h"
#include <EstereoCameraCapturer.h>
#include <QPixmap>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <QtOpenCVInteroperability.h>
#include <CvHelper.h>
#include "Worker.h"

#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>

static const int CAM_WIDTH = 640;
static const int CAM_HEIGHT = 680;

static EstereoCameraCapturer *estereoCameraCapturerPtr1 = 0;
static QTimer *cameras_timer = 0;

void StartEstereoCameraCapturer()
{
	if (estereoCameraCapturerPtr1) {
		estereoCameraCapturerPtr1->Start();
	}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_worker(nullptr),
    m_arduinoController(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_arduinoController = new ArduinoController;

	//OpenCV Cameras
	if (true) {
		cameras_timer = new QTimer;
		cameras_timer->start(33);

		m_estereoCameraCapturer = new EstereoCameraCapturer(CAM_WIDTH*0.5, CAM_HEIGHT*0.5, cameras_timer);

		estereoCameraCapturerPtr1 = m_estereoCameraCapturer;
		std::thread t1(StartEstereoCameraCapturer);
		t1.join();

		connect(estereoCameraCapturerPtr1, SIGNAL(imageCaptured()), this, SLOT(ShowImageCaptured()));
	}
	
    //CamerasQt
	//InitializeStereoCameraViewersQt()

    //Angles visualizers
	InitializeAnglesVisualizers();
	
    //Worker
	InitializeWorker();

   connect(ui->actionCameras_viewer, SIGNAL(toggled(bool)), SLOT(SetOpenCVCamerasVisible(bool)));
   connect(ui->actionAngles_viewer, SIGNAL(toggled(bool)), SLOT(SetAnglesViewerVisible(bool)));
    
}

void MainWindow::InitializeStereoCameraViewersQt() {
	CameraWidget *cw1 = new CameraWidget();
	CameraWidget *cw2 = new CameraWidget();

	ui->horizontalLayout1->addWidget(cw1);
	ui->horizontalLayout1->addWidget(cw2);

	cw1->SetCamera(0);
	cw2->SetCamera(1);
}

void MainWindow::InitializeAnglesVisualizers() {
	m_angleViewerControllerWidget1 = new AngleViewerControllerWidget(ANGLE_CONTROLLER_HORIZONTAL);
	ui->horizontalLayout2->addWidget(m_angleViewerControllerWidget1);

	m_angleViewerControllerWidget2 = new AngleViewerControllerWidget(ANGLE_CONTROLLER_VERTICAL);
	ui->horizontalLayout2->addWidget(m_angleViewerControllerWidget2);

	connect(m_angleViewerControllerWidget1, SIGNAL(writeOnArduino(const char*)), m_arduinoController, SLOT(Write(const char*)));
	connect(m_angleViewerControllerWidget2, SIGNAL(writeOnArduino(const char*)), m_arduinoController, SLOT(Write(const char*)));
}

void MainWindow::InitializeWorker() {
   m_worker = new Worker;
	m_workerThread = new QThread;
	m_worker->moveToThread(m_workerThread);
	connect(m_workerThread, SIGNAL(started()), m_worker, SLOT(process()));
	connect(m_worker, SIGNAL(finished()), m_workerThread, SLOT(quit()));
	connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
	connect(m_workerThread, SIGNAL(finished()), m_workerThread, SLOT(deleteLater()));

	connect(m_worker, SIGNAL(readArduinoTime()), this, SLOT(ReadArduinoData()));
	m_workerThread->start();
}

MainWindow::~MainWindow()
{
    if (m_arduinoController) {
       delete m_arduinoController;
        m_arduinoController = nullptr;
    }

    delete ui;
}

void MainWindow::ReadArduinoData()
{
    if( m_arduinoController->Write("3\n") == 0)
    {
        QByteArray data;
        if(m_arduinoController->Read(data)==0) {
            //QString arduOutAsStr = QString::fromStdString(data.toStdString());
            QJsonDocument doc = QJsonDocument::fromJson(data);
            //get the jsonObject
            QJsonObject jObject = doc.object();

            QVariantMap mainMap = jObject.toVariantMap();
            //QVariantMap mainMap = QVariant(data).toMap();
            const QList<QVariant> &stepperVariantList = mainMap["steppers"].toList();
            if (stepperVariantList.isEmpty())
                return;

            m_angleViewerControllerWidget1->UpdateGUIFromArduinoData(stepperVariantList[0]);
            m_angleViewerControllerWidget2->UpdateGUIFromArduinoData(stepperVariantList[1]);
        }
    }
}


void MainWindow::ReadArduinoDataAsString()
{
    if( m_arduinoController->Write("3\n") == 0)
    {
        QByteArray data;
        if(m_arduinoController->Read(data)==0) {
            QString arduOutAsStr = QString::fromStdString(data.toStdString());
            m_angleViewerControllerWidget1->UpdateGUIFromArduinoData(arduOutAsStr);
        }
    }
}

void MainWindow::ShowImageCaptured()
{
	cv::Mat cvImg1, cvImg2;

	if (!estereoCameraCapturerPtr1)
		return;

	estereoCameraCapturerPtr1->GetCapture(cvImg1, cvImg2);

	QImage qimage1 = QtOpenCV::cvMatToQImage(cvImg1);
	this->ui->leftImageLabel->setPixmap(QPixmap::fromImage(qimage1));

	QImage qimage2 = QtOpenCV::cvMatToQImage(cvImg2);
	this->ui->rightImageLabel->setPixmap(QPixmap::fromImage(qimage2));

   //ShowCannyImageCaptured(&cvImg1, &cvImg2);
   ShowDeepImageCaptured(&cvImg1, &cvImg2);
}

void MainWindow::ShowCannyImageCaptured(const cv::Mat *cvImg1, const cv::Mat *cvImg2)
{
	cv::Mat cvCanny1, cvCanny2;

	CvHelper::Canny(*cvImg1, cvCanny1);
	CvHelper::Canny(*cvImg2, cvCanny2);

	QImage qimageCanny1 = QtOpenCV::cvMatToQImage(cvCanny1);
	this->ui->leftImageLabel_2->setPixmap(QPixmap::fromImage(qimageCanny1));

	QImage qimageCanny2 = QtOpenCV::cvMatToQImage(cvCanny2);
	this->ui->rightImageLabel_2->setPixmap(QPixmap::fromImage(qimageCanny2));
}

void MainWindow::ShowDeepImageCaptured(const cv::Mat *cvImg1, const cv::Mat *cvImg2)
{
      //Disparity
   cv::Mat greyLMat, greyRMat;
   cv::cvtColor(*cvImg1, greyLMat, CV_BGR2GRAY);
   cv::cvtColor(*cvImg2, greyRMat, CV_BGR2GRAY);
   cv::Mat disp = greyLMat.clone();
   greyLMat.convertTo(greyLMat, CV_8UC1);
   greyRMat.convertTo(greyRMat, CV_8UC1);
   disp.convertTo(disp, CV_16UC1);

   cv::Ptr<cv::StereoBM> stereo = cv::StereoBM::create(0, 21);
   stereo->compute(greyLMat, greyRMat, disp);
   disp.convertTo(disp, CV_8UC1);
   //normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
   QImage qimageDeep1 = QtOpenCV::cvMatToQImage(disp);

   this->ui->leftImageLabel_2->setPixmap(QPixmap::fromImage(qimageDeep1));
   this->ui->rightImageLabel_2->setPixmap(QPixmap::fromImage(qimageDeep1));
}


void MainWindow::SetOpenCVCamerasVisible(bool value)
{
   if (value)
      m_estereoCameraCapturer->Stop();
   else
      m_estereoCameraCapturer->Start();

   ui->leftImageLabel->setVisible(value);
   ui->rightImageLabel->setVisible(value);
   ui->leftImageLabel_2->setVisible(value);
   ui->rightImageLabel_2->setVisible(value);
}

void MainWindow::SetAnglesViewerVisible(bool value)
{
   m_angleViewerControllerWidget1->setVisible(value);
   m_angleViewerControllerWidget2->setVisible(value);
   m_worker->SetEnabled(value); 
}

