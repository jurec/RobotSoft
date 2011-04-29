#include "moveforwardcommand.h"
#include <robot.h>
#include <QDebug>
moveForwardCommand::moveForwardCommand(QObject *parent) :
    command(parent)
{
    setName("MOVE FORWARD");
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
    myRobot->setRobotCenter(QPointF(
            qCos(myRobot->getAngle())*distance,
            qSin(myRobot->getAngle())*distance));
}
void moveForwardCommand::execute(robot *myRobot)
{

}
