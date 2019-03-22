#ifndef WORKER_H
#define WORKER_H

#include <QObject>
class ArduinoController;

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();

    ~Worker();
public slots:
    void process();

signals:
    void readArduinoTime();

    void finished();

    void error(QString err);
private:
    // add your variables here
};
#endif // WORKER_H
