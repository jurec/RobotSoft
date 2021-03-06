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
    //������� ����������� ��������� ����
    scene->addRect(startzones[0].x(),startzones[0].y(),startzoneSize.width(),startzoneSize.height(),QPen(Qt::gray),QBrush(Qt::gray));
    field[0][0].setRect(startzones[0].x(),startzones[0].y(),startzoneSize.width(),startzoneSize.height());
    field[0][0].setType(cell::start);
    scene->addRect(startzones[1].x(),startzones[1].y(),startzoneSize.width(),startzoneSize.height(),QPen(Qt::gray),QBrush(Qt::gray));
    field[0][M-1].setRect(startzones[1].x(),startzones[1].y(),startzoneSize.width(),startzoneSize.height());
    field[0][M-1].setType(cell::start);
    scene->addRect(greenzones[0].x(),greenzones[0].y(),greenzoneSize.width(),greenzoneSize.height(),QPen(Qt::green),QBrush(Qt::green));
    for(int i=1;i<M;i++)
    {
        int dk=0;
        (i==1)?dk=5:dk=0;
            field[i][0].setRect(greenzones[0].x(),greenzones[0].y(),cellSize.width()+10,cellSize.height()-dk);
            field[i][0].setType(cell::green);
    }
    for(int i=1;i<M;i++)
    {
        int dk=0;
        (i==1)?dk=5:dk=0;
            field[i][N-1].setRect(greenzones[0].x(),greenzones[0].y(),cellSize.width()+10,cellSize.height()-dk);
            field[i][N-1].setType(cell::green);
    }

    scene->addRect(greenzones[1].x(),greenzones[1].y(),greenzoneSize.width(),greenzoneSize.height(),QPen(Qt::green),QBrush(Qt::green));

    scene->addRect(blackLines[0].x(),blackLines[0].y(),blackLineSize.width(),blackLineSize.height(),QPen(Qt::black),QBrush(Qt::black));
    scene->addRect(blackLines[1].x(),blackLines[1].y(),blackLineSize.width(),blackLineSize.height(),QPen(Qt::black),QBrush(Qt::black));
//����� ������� � ����� ������
    bool fl=true;
    for(int i=0;i<M;i++)
    { fl=!fl;
       for(int j=0;j<M;j++)
        {
           cellCoordinates[i][j].setX(45+j*cellSize.width());
           cellCoordinates[i][j].setY(0+i*cellSize.height());
           field[i+1][j+1].setRect(cellCoordinates[i][j].x(),cellCoordinates[i][j].y(),cellSize.width(),cellSize.height());
           //�������� �������� ������ � ���������� ����
           if((i==1 && (j==1 || j==4)) || (i==3 && (j==1 || j==4)) || (i==5 && (j==2 || j==3)))
               field[i+1][j+1].setType(cell::bonus);
           else
              field[i+1][j+1].setType(cell::game);
           if(i==5 && (j!=2 || j!=3))
               field[i+1][j+1].setType(cell::locked);
            field[i+1][j+1].setRect(cellCoordinates[i][j].x(),cellCoordinates[i][j].y(),cellSize.width(),cellSize.height());

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
void robotField::calculateCenter()//������������� ���������� ������
{
}

void robotField::drawRobot(robotParameters *robotParameter)
{

   // robotCenter=b->robotCenter;
    robotCenter = new QPointF(robotParameter->getRobotCenter());
    robotRect = new QRectF(robotParameter->getRobotModel());
    isRed=robotParameter->isRed();
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
        moveRobot(-5);

    if(event->key()==Qt::Key_Down)
        moveRobot(5);

    if(event->key()==Qt::Key_Right)
        rotateRobot(10);;

    if(event->key()==Qt::Key_Left)
        rotateRobot(-10);
    if(event->key()==Qt::Key_0)
        rotateRobot(15);
}

//void robotField::moveRobot(int x, int y)
//{
//    robot->moveBy(x,y);
//    // ���������� ������ ��������� ����������
//    calculateCenter();
//   // robot->setTransformOriginPoint(*robotCenter);
//    qDebug()<<robot->x()<<" "<<robot->y()<<"\n";
//    //qDebug()<<robotCenter->x()<<" "<<robotCenter->y()<<"\n";
//}
void robotField::moveRobot(int distance)
{
    robot->moveBy(qCos(degrees(robot->rotation()))*distance,qSin(degrees(robot->rotation())*distance));
    qDebug()<<robot->x()<<" "<<robot->y()<<"\n";
}

void robotField::rotateRobot(int angle)
{
    robot->setRotation(robot->rotation()+angle);
    qDebug()<<robot->rotation();
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
