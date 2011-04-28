#include "robotfield.h"


#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QKeyEvent>
robotField::robotField(QGraphicsView *parent) :
    QGraphicsView(parent)
{
    //connect(this,SIGNAL(mousePressed()),this,SLOT(drawRobot()));
    initZonesSize();
    //widget = new QGraphicsWidget();
    scene = new QGraphicsScene();
 //   scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    scene->setSceneRect(0,0,300,210);
    // setViewport();
   // setMinimumSize(300,210);
  //  setWindowTitle(tr("Simulation robot"));
 //   setRenderHints(QPainter::Antialiasing);
    setScene(scene);
    drawField();
    //
 //   drawRobot();
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

void robotField::drawRobot()
{


    robot->setTransformOriginPoint(*robotCenter);
    robot->setFlag(QGraphicsItem::ItemIsMovable);
    robot->setData(0,"Robot");
   //robot->setRotation(robot->rotation()+15);
   //<<robot->x()<<" "<<robot->y();
}
void robotField::calculateCenter()//Пересчитывает координаты центра
{
   robotCenter->setX(
           (robotRect->topLeft().x()+robotRect->bottomRight().x())/2);
   robotCenter->setY(
           (robotRect->topLeft().y()+robotRect->bottomRight().y())/2);
}

void robotField::drawRobot(robotParameters *robotParameter)
{

   // robotCenter=b->robotCenter;
    robotCenter = new QPointF(robotParameter->robotCenter);
    robotRect = new QRect(robotParameter->robotModel);
    isRed=robotParameter->isRed;
    if(!isRed)
    {
       robot=(scene->addRect(*robotRect,QPen(Qt::blue),QBrush(Qt::white)));
       robotRect->setX(277);
       robotRect->setY(0);
       robotRect->setWidth(23);
       robotRect->setHeight(34);
    }
    else
    {
        robot=(scene->addRect(*robotRect,QPen(Qt::red),QBrush(Qt::white)));
    }
      //  robotRect->setX(0);
    robotRect->setY(0);
    drawRobot();
}
void robotField::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Up)
        moveRobot(10,0);
}

void robotField::moveRobot(int x, int y)
{
    robot->moveBy(x,y);
    // Перемещаем робота обновляем координаты
    calculateCenter();
    qDebug()<<robot->x()<<" "<<robot->y()<<"\n";
}

//void robotField::drawRobot(QPointF &center)
//{
//     robotCenter=new QPointF(center);//11.5,17
//     drawRobot();
//}
//void robotField::drawRobot(qreal xCenter, qreal yCenter)
//{
//    robotCenter=new QPointF(xCenter,yCenter);
//    drawRobot();
//}

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


robotField::~robotField()
{
   delete scene;
}
