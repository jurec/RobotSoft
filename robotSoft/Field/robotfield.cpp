#include "robotfield.h"
#include <QtGui/QWidget>
#include <QPainter>
#include <QColor>
#include <QSize>
#include <QGraphicsRectItem>
robotField::robotField(QGraphicsView *parent) :
    QGraphicsView(parent)
{
    initZonesSize();

    scene = new QGraphicsScene(this);
   // scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    scene->setSceneRect(0,0,300,210);
    setScene(scene);
    setMinimumSize(300,210);
    setWindowTitle(tr("Simulation robot"));
    drawField();
    robot=(scene->addRect(100,100,10,10,QPen(Qt::yellow),QBrush(Qt::yellow)));
    robot->setData(0,"Robot");
    // *(scene->addRect(10,10,10,10));

}
void robotField::drawField()
{
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    scene->addRect(startzones[0].x(),startzones[0].y(),startzoneSize.width(),startzoneSize.height(),QPen(Qt::gray),QBrush(Qt::gray));
    scene->addRect(startzones[1].x(),startzones[1].y(),startzoneSize.width(),startzoneSize.height(),QPen(Qt::gray),QBrush(Qt::gray));

    scene->addRect(greenzones[0].x(),greenzones[0].y(),greenzoneSize.width(),greenzoneSize.height(),QPen(Qt::green),QBrush(Qt::green));
    scene->addRect(greenzones[1].x(),greenzones[1].y(),greenzoneSize.width(),greenzoneSize.height(),QPen(Qt::green),QBrush(Qt::green));

    scene->addRect(blackLines[0].x(),blackLines[0].y(),blackLineSize.width(),blackLineSize.height(),QPen(Qt::black),QBrush(Qt::black));
    scene->addRect(blackLines[1].x(),blackLines[1].y(),blackLineSize.width(),blackLineSize.height(),QPen(Qt::black),QBrush(Qt::black));

    bool fl=true;
    for(int i=0;i<M;i++)
    { fl=!fl;
       for(int j=0;j<M;j++)
        {
           cellCoordinates[i][j].setX(45+j*cellSize.width());
           cellCoordinates[i][j].setY(0+i*cellSize.height());
        if(fl)
             scene->addRect(cellCoordinates[i][j].x(),cellCoordinates[i][j].y(),cellSize.width(),cellSize.height(),QPen(Qt::red),QBrush(Qt::red));
        else
            scene->addRect(cellCoordinates[i][j].x(),cellCoordinates[i][j].y(),cellSize.width(),cellSize.height(),QPen(Qt::blue),QBrush(Qt::blue));
        fl=!fl;
    }
   }
}
void robotField::mouseReleaseEvent(QMouseEvent *event)
{
    drawRobot();
}

void robotField::drawRobot()
{
   // robot->translate(20,20);
       // robot->rotate(-2.6);
}

void robotField::initZonesSize()
{
    cellSize.setHeight(35);
    cellSize.setWidth(35);

    startzoneSize.setHeight(40);
    startzoneSize.setWidth(40);
    startzones[0].setX(0);
    startzones[0].setY(0);
    startzones[1].setX(260);
    startzones[1].setY(0);

    greenzoneSize.setHeight(170);
    greenzoneSize.setWidth(40);
    greenzones[0].setX(0);
    greenzones[0].setY(40);
    greenzones[1].setX(260);
    greenzones[1].setY(40);

    blackLineSize.setHeight(210);
    blackLineSize.setWidth(5);
    blackLines[0].setX(40);
    blackLines[0].setY(0);
    blackLines[1].setX(255);
    blackLines[1].setY(0);
}

void robotField::changeFlag()
{
    flag= !flag;
}

