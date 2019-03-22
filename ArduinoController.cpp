#include "ArduinoController.h"
#include <QSerialPort>
#include <QSerialPortInfo>


ArduinoController::ArduinoController()
{
    arduino = 0;
    FindAndConnect();
}

void ArduinoController::FindAndConnect()
{
    _Initialize();

    QString deviceSerialPortName = "";
    int productID = 0;

    _Find(deviceSerialPortName, productID);

    if(available){
        _Connect(deviceSerialPortName, productID);
    }
    else{
        emit notFoundSignal();
    }
}

void ArduinoController::_Initialize()
{
    connected = false;
    available = false;
    arduino_port = "";
    if (arduino) {
        delete arduino;
        arduino = 0;
    }
    arduino = new QSerialPort;
}

bool ArduinoController::_Find(QString &deviceSerialPortName, int &productID)
{
    available = false;
    //qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        //qDebug() << "Identificador del fabricante (VENDOR ID): " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            //qDebug() << "ID Vendedor " << serialPortInfo.vendorIdentifier();
            //qDebug() << "ID Producto: " << serialPortInfo.productIdentifier();

            if(serialPortInfo.productIdentifier() == arduino_UNO || serialPortInfo.productIdentifier() == arduino_MEGA){
                deviceSerialPortName = serialPortInfo.portName();
                productID = serialPortInfo.productIdentifier();
                available = true;
            }
        }
    }

    return available;
}

bool ArduinoController::_Connect(const QString &deviceSerialPortName, const int &productID)
{
    arduino_port = deviceSerialPortName;
    arduino ->setPortName(arduino_port);
    arduino->open(QIODevice::ReadWrite);
    arduino->setDataBits(QSerialPort::Data8);
    arduino ->setBaudRate(QSerialPort::Baud9600);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    //qDebug() << "Producto: " << productID;

    if(productID == 67) {
        connected = true;
        emit connectedSignal("Arduino UNO R3 conectado");
    }
    else if(productID == 68) {
        connected = true;
        emit connectedSignal("Arduino Mega conectado");
    }
    else
    {
        connected = false;
        emit connectedSignal("Error 3");
    }

    return connected;
}

int ArduinoController::Write(const char *data)
{
    if(available && arduino->isWritable()) {
        arduino->write(data);
        return 0;
    }

    return 1;
}

int ArduinoController::Read(QByteArray &data)
{
    if(available && arduino->isReadable()) {
        if(arduino->isReadable()) {
            // QByteArray datoLeido = arduino->read(2);
            data = arduino->readAll();
            // int DatoEntero = datoLeido.toHex().toInt(0,16);
            return 0;
        }
    }

    return 1;
}

ArduinoController::~ArduinoController()
{
    if (arduino) {
        delete arduino;
        arduino = 0;
    }
}
