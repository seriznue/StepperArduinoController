#ifndef ANGLEVIEWERCONTROLLERWIDGET_H
#define ANGLEVIEWERCONTROLLERWIDGET_H

#include <QWidget>
#include <QVariant>

typedef enum {
    ANGLE_CONTROLLER_HORIZONTAL = 0,
    ANGLE_CONTROLLER_VERTICAL = 1
} AngleControllerType;

class ArduinoController;
class AngleViewerWidget;

namespace Ui {
class AngleViewerControllerWidget;
}

class AngleViewerControllerWidget : public QWidget
{
    Q_OBJECT

    AngleControllerType m_angleControllerType;
    AngleViewerWidget *m_angleViewerWidget;

public:
    explicit AngleViewerControllerWidget(AngleControllerType angleControllerType, QWidget *parent = nullptr);
    ~AngleViewerControllerWidget();
    int GetSliderValue() const;

public slots:
    void UpdateGUIFromArduinoData(QString arduOutAsStr);
    void UpdateGUIFromArduinoData(const QVariant &arduOutAsVariant);
    void Advance();
    void Regress();
signals:
    void writeOnArduino(const char*);

private:
    Ui::AngleViewerControllerWidget *ui;
};

#endif // ANGLEVIEWERCONTROLLERWIDGET_H
