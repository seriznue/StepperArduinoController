#ifndef QTARDUINOCONTROLLER_H
#define QTARDUINOCONTROLLER_H

#include "qtarduinocontrollerlib_global.h"
#include <QSerialPort>

class ArduinoController : public QObject
{
    Q_OBJECT

    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_UNO = 66;
    static const quint16 arduino_MEGA = 67;
    QString arduino_port;
    bool available;
    bool connected;

    void _Initialize();

public:
    ArduinoController();

    virtual ~ArduinoController();

private:
    bool _Find(QString &deviceSerialPortName ,int &productID);
    bool _Connect(const QString &deviceSerialPortName, const int &productID);

public slots:
    void FindAndConnect();
    int Write(const char *data);
    int Read(QByteArray &data);

signals:
    void foundSignal();
    void connectedSignal(const QString &);
    void notFoundSignal();
    void notConnectedSignal(const QString &);
};

#endif // QTARDUINOCONTROLLER_H
