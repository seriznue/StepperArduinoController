#include "Worker.h"
#include <QDebug>
#include <QThread>
#include "ArduinoController.h"
#include "MainWindow.h"

Worker::Worker() : m_enabled(true)
{
    // you could copy data from constructor arguments to internal variables here.
}

Worker::~Worker() {
    // free resources
}

void Worker::process() { // Process. Start processing data.

    while(true) {
       if (m_enabled) {
          emit readArduinoTime();
       }
       
        QThread::msleep(500);
    }

    //emit finished();
}
