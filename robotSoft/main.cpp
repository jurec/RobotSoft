#include <QtCore/QCoreApplication>
#include <Field/robotfield.h>
#include <QApplication>
#include <iostream>
#include <robot.h>
#include <QGraphicsView>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robot *myRobot=new robot();

    robotField *myF=new robotField;
    myRobot->setWidth(23);
    myRobot->setLength(34);
    myRobot->setRobotCenter();
    myRobot->setRed();
    myF->drawRobot(myRobot->getRobotParametrs());
    myF->show();
//    myF->show();
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
