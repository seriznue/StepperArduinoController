#ifndef ANGLEVIEWERWIDGET_H
#define ANGLEVIEWERWIDGET_H

#include <QGraphicsView>

class AngleViewerWidget : public QGraphicsView
{
    Q_OBJECT

    QGraphicsPolygonItem *m_polygonItem;

    double m_angle;

public:
    AngleViewerWidget();

    virtual ~AngleViewerWidget();

    void SetAngle(double angle);
public slots:
    void IncrementAngle();

    void DecrementAngle();

private:
};

#endif // ANGLEVIEWERWIDGET_H
