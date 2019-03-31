#include "Worker.h"
#include <QDebug>
#include <QThread>
#include "ArduinoController.h"
#include "MainWindow.h"

Worker::Worker()
{
    // you could copy data from constructor arguments to internal variables here.
}

Worker::~Worker() {
    // free resources
}

void Worker::process() { // Process. Start processing data.

    while(true) {
        emit readArduinoTime();

        QThread::msleep(500);
    }

    //emit finished();
}
