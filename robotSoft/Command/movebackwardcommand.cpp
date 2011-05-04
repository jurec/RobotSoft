#include "movebackwardcommand.h"
#include <robot.h>
moveBackwardCommand::moveBackwardCommand(QObject *parent) :
    command(parent)
{
    setName("MOVE BACKWARD ");
}
moveBackwardCommand::moveBackwardCommand(QObject *parent, int distance):
        command(parent)
{
    setName("MOVE BACKWARD ");
    setDistance(distance);
}

void moveBackwardCommand::execute(robot *myRobot, int distance)
{
    //Движение вперед
    setName(getName()+QString::number(distance));
    if(myRobot->getRobotEngine()!=NULL)
    {
        if(myRobot->getRobotEngine()->isOpen())
            myRobot->getRobotEngine()->sendCommand(BACKWARD,distance);
        else
            qDebug("Engine not connected");
    }
    //Дальше надо бы пересчитать координаты центра робота
    myRobot->getRobotParametrs()->moveRobot(distance);
//            qCos(myRobot->getRobotParametrs()->getAngle())*distance,
//            qSin(myRobot->getRobotParametrs()->getAngle())*distance;
}
