#ifndef ANGLEVIEWERCONTROLLERWIDGET_H
#define ANGLEVIEWERCONTROLLERWIDGET_H

#include <QWidget>

class ArduinoController;
class AngleViewerWidget;

namespace Ui {
class AngleViewerControllerWidget;
}

class AngleViewerControllerWidget : public QWidget
{
    Q_OBJECT

    ArduinoController *m_arduinoController;
    AngleViewerWidget *m_angleViewerWidget;

public:
    explicit AngleViewerControllerWidget(ArduinoController *arduinoController, QWidget *parent = nullptr);
    ~AngleViewerControllerWidget();

public slots:
    void UpdateGUIFromArduinoData(QString arduOutAsStr);
    void Advance();
    void Regress();

private:
    Ui::AngleViewerControllerWidget *ui;
};

#endif // ANGLEVIEWERCONTROLLERWIDGET_H
