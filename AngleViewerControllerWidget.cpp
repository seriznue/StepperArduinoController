#include "AngleViewerControllerWidget.h"
#include "AngleViewerWidget.h"
#include "ui_AngleViewerControllerWidget.h"

static const char* ADVANCE_H = "1\n";
static const char* REGRESS_H = "2\n";
static const char* ADVANCE_V = "4\n";
static const char* REGRESS_V = "5\n";

//static const char* READ    = "3\n";

AngleViewerControllerWidget::AngleViewerControllerWidget(AngleControllerType angleControllerType, QWidget *parent) :
    QWidget(parent),
    m_angleControllerType(angleControllerType),
    m_angleViewerWidget(new AngleViewerWidget()),
    ui(new Ui::AngleViewerControllerWidget)
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(m_angleViewerWidget);
    connect(ui->plusPushButton, SIGNAL(clicked()), this, SLOT(Advance()));
    connect(ui->minusPushButton, SIGNAL(clicked()), this, SLOT(Regress()));
}

AngleViewerControllerWidget::~AngleViewerControllerWidget()
{
    if (m_angleViewerWidget) {
        delete m_angleViewerWidget;
        m_angleViewerWidget = nullptr;
    }

    delete ui;
}

void AngleViewerControllerWidget::UpdateGUIFromArduinoData(QString arduOutAsStr)
{
    for(QString token: arduOutAsStr.split(';')) {
        if(token.contains("angle=")) {
            token = token.remove("angle=");
            ui->angleLabel->setText("Angle = " + token + " º");
            m_angleViewerWidget->SetAngle(token.toDouble());
        }
        else if(token.contains("steps=")) {
            token = token.remove("steps=");
            ui->stepLabel->setText("Steps = " + token);
        }
        else if(token.contains("value=")) {
            token = token.remove("value=");
            ui->valueLabel->setText("Pothenciometter value = " + token);
            ui->pothenciometterHorizontalSlider->setValue(token.toInt());
        }
        else if(token.contains("steps_per_round=")) {
            token = token.remove("steps_per_round=");
            ui->stepsPerRoundLabel->setText("Steps per round : " + token);
        }
        else if(token.contains("bobinas=")) {
            token = token.remove("bobinas=");
            ui->bobbinsLabel->setText("Bobbins : " + token);
        }
    }
}

void AngleViewerControllerWidget::UpdateGUIFromArduinoData(const QVariant &arduOutAsVariant)
{
    const QVariantMap &map = arduOutAsVariant.toMap();

    ui->angleLabel->setText("Angle = " + map["angle"].toString() + " º");
    m_angleViewerWidget->SetAngle(map["angle"].toDouble());
    ui->stepLabel->setText("Steps = " + map["steps"].toString());
    ui->valueLabel->setText("Pothenciometter value = " + map["value"].toString());
    ui->pothenciometterHorizontalSlider->setValue(map["value"].toInt());
    ui->stepsPerRoundLabel->setText("Steps per round : " + map["steps_per_round"].toString());
    ui->bobbinsLabel->setText("Bobbins : " + map["bobinas"].toString());
}

void AngleViewerControllerWidget::Advance()
{
    if (m_angleControllerType == ANGLE_CONTROLLER_HORIZONTAL)
        emit writeOnArduino(ADVANCE_H);
    else
        emit writeOnArduino(ADVANCE_V);
}

void AngleViewerControllerWidget::Regress()
{
    if (m_angleControllerType == ANGLE_CONTROLLER_HORIZONTAL)
        emit writeOnArduino(REGRESS_H);
    else
        emit writeOnArduino(REGRESS_V);
}

int AngleViewerControllerWidget::GetSliderValue() const
{
     return ui->pothenciometterHorizontalSlider->value();
}
