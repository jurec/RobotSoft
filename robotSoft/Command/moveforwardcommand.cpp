#include "moveforwardcommand.h"
#include <robot.h>
#include <QDebug>
moveForwardCommand::moveForwardCommand(QObject *parent) :
    command(parent)
{
    setName("MOVE FORWARD");
}
moveForwardCommand::moveForwardCommand(QObject *parent, int distance) :
    command(parent)
{
    setName("MOVE FORWARD "+QString::number(distance));
    setDistance(distance);
}

void moveForwardCommand::execute(robot *myRobot, int distance)
{
    //Движение вперед
    setName(getName()+distance);
    if(myRobot->getRobotEngine()!=NULL)
    {
        if(myRobot->getRobotEngine()->isOpen())
            myRobot->getRobotEngine()->sendCommand(FORWARD,distance);
        else
            qDebug("Engine not connected");
    }
    //Дальше надо бы пересчитать координаты центра робота
    myRobot->getRobotParametrs()->moveRobot(distance);
}
