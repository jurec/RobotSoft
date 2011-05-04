#include "rotatecommand.h"

rotateCommand::rotateCommand(QObject *parent) :
    command(parent)
{
    setName("ROTATE");
}
rotateCommand::rotateCommand(QObject *parent, int distance) :
    command(parent)
{
    setName("ROTATE "+QString::number(distance));
    setDistance(distance);
}
void rotateCommand::execute(robot *myRobot, int distance)
{
    //Движение вперед
    setName(getName()+distance);
    if(myRobot->getRobotEngine()!=NULL)
    {
        if(myRobot->getRobotEngine()->isOpen())
            if(distance>0)
                myRobot->getRobotEngine()->sendCommand(LEFT,distance);
            else
                myRobot->getRobotEngine()->sendCommand(RIGHT,-1*distance);
        else
            qDebug("Engine not connected");
    }
    //Дальше надо бы пересчитать координаты центра робота
    myRobot->getRobotParametrs()->rotateRobot(distance);
}
