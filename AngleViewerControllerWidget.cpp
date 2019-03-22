#include "AngleViewerControllerWidget.h"
#include "AngleViewerWidget.h"
#include "ArduinoController.h"
#include "ui_AngleViewerControllerWidget.h"

static const char* ADVANCE_CHAR = "1\n";
static const char* REGRESS_CHAR = "2\n";
static const char* READ_CHAR    = "3\n";

AngleViewerControllerWidget::AngleViewerControllerWidget(ArduinoController *arduinoController, QWidget *parent) :
    QWidget(parent),
    m_arduinoController(arduinoController),
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

void AngleViewerControllerWidget::Advance()
{
    m_arduinoController->Write(ADVANCE_CHAR);
}

void AngleViewerControllerWidget::Regress()
{
    m_arduinoController->Write(REGRESS_CHAR);
}
