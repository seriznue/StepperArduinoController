#include "AngleViewerWidget.h"
#include "QGraphicsPixmapItem"

AngleViewerWidget::AngleViewerWidget(): QGraphicsView (),
    m_polygonItem(nullptr),
    m_angle(0)
{   
    QImage image;

    image.load(":/Resources/circle-with-degrees.jpg");

    image = image.scaled(350, 350);

    QPixmap pixmap = QPixmap::fromImage(image);

    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);

    QGraphicsScene *scene = new QGraphicsScene;

    this->setScene(scene);

    scene->addItem(pixmapItem);

    QPolygon p;

    p << QPoint(0, 0) << QPoint(5, 5) << QPoint(0, -130) << QPoint(-5, 5);

    m_polygonItem = new QGraphicsPolygonItem(p);

    m_polygonItem->setPos(image.width()*0.5, image.height()*0.5);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFixedSize(image.width(), image.height());

    scene->addItem(m_polygonItem);
}

void AngleViewerWidget::SetAngle(double angle) {

    m_angle = angle;

    m_polygonItem->setRotation(angle);
}

void AngleViewerWidget::IncrementAngle() {

    ++m_angle;

    m_polygonItem->setRotation(m_angle);
}

void AngleViewerWidget::DecrementAngle() {

    --m_angle;

    m_polygonItem->setRotation(m_angle);
}

AngleViewerWidget::~AngleViewerWidget()
{

}
