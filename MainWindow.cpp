#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ArduinoController.h"
#include "AngleViewerControllerWidget.h"
#include <QThread>
#include "Worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_arduinoController(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_arduinoController = new ArduinoController;

    Worker* worker = new Worker();

    m_angleViewerControllerWidget1 = new AngleViewerControllerWidget(m_arduinoController);
    ui->horizontalLayout->addWidget(m_angleViewerControllerWidget1);

    m_angleViewerControllerWidget2 = new AngleViewerControllerWidget(m_arduinoController);
    ui->horizontalLayout->addWidget(m_angleViewerControllerWidget2);

    QThread* thread = new QThread;
    worker->moveToThread(thread);

    connect(thread, SIGNAL (started()),  worker, SLOT (process()));
    connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));

    connect(worker, SIGNAL (readArduinoTime()), this, SLOT (ReadArduinoData()));
    thread->start();
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
            QString arduOutAsStr = QString::fromStdString(data.toStdString());
            m_angleViewerControllerWidget1->UpdateGUIFromArduinoData(arduOutAsStr);
            m_angleViewerControllerWidget2->UpdateGUIFromArduinoData(arduOutAsStr);
        }
    }
}
