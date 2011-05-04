#include <QtCore/QCoreApplication>
#include <Field/robotfield.h>
#include <QApplication>
#include <iostream>
#include <robot.h>
#include <QGraphicsView>
#include <robotparameters.h>
#include <Command/moveforwardcommand.h>
#include <Command/movebackwardcommand.h>
#include <Command/rotatecommand.h>
#include <Command/stopcommand.h>
#include <Command/pickupcommand.h>
#include <QDebug>
#include <taskrunner.h>
#include <Field/robotfield.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robot *myRobot=new robot();

    robotField *myF=new robotField;
    myRobot->init_devices();
    myRobot->getRobotParametrs()->setAngle(0);
    myRobot->getRobotParametrs()->setRobotCenter(QPointF(11.5,17));
    myRobot->getRobotParametrs()->setTeamRed();
    myRobot->getRobotParametrs()->setRobotRect(QRectF(QPointF(0,0),QSize(23,34)));

    taskRunner robotStrategy;
    robotStrategy.addTask(new rotateCommand(0,30));
    robotStrategy.addTask(new moveForwardCommand(0,50));

    robotStrategy.addTask(new stopCommand(0,0));

    robotStrategy.runTasks(myRobot);
    myRobot->getRobotParametrs()->getRobotModelMatrix();
    myRobot->getRobotParametrs()->getRobotCenterMatrix();

    QMatrix qm;
    qm.map(QPointF(10,10));
    qm.rotate(45);
    qm.translate(30,30);
//    myRobot->setWidth(23);
//    myRobot->setLength(34);
//    myRobot->setRobotCenter(QPointF(11.5,17));
//    myRobot->setRed();
 //   myF->drawRobot(myRobot->getRobotParametrs());
 //   moveForwardCommand move;
//    move.execute(myRobot,255);
//    myF->show();
   // myF->moveRobot(50);
  //  myF->show();
\
//     std::cout<<sizeof(myF);
   // myF.drawRobot();
   // myF.drawRobot();
  //  QWidget widget;

//    QGraphicsScene scene;

//    scene.setSceneRect(0,0,500,500);

//    scene.addRect(0,0,10,10,QPen(), QBrush(QColor(Qt::black)));

//    QGraphicsView view;

//    view.setScene(&scene);

//    view.show();
    // delete b;

    return a.exec();
}
