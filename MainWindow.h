#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AngleViewerControllerWidget.h"
#include "AngleViewerWidget.h"

namespace Ui {
class MainWindow;
}

class ArduinoController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ArduinoController *m_arduinoController;
    AngleViewerControllerWidget *m_angleViewerControllerWidget1;
    AngleViewerControllerWidget *m_angleViewerControllerWidget2;
    AngleViewerControllerWidget *m_angleViewerControllerWidget3;
    AngleViewerControllerWidget *m_angleViewerControllerWidget4;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void ReadArduinoData();

};

#endif // MAINWINDOW_H
