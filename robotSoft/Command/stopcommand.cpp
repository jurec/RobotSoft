#include "stopcommand.h"

stopCommand::stopCommand(QObject *parent) :
        command(parent)
{
    setName("STOP");
}
stopCommand::stopCommand(QObject *parent, int distance) :
        command(parent)
{
    setName("STOP");
    setDistance(distance);
}
void stopCommand::execute(robot *myRobot)
{
    //Движение вперед
    setName(getName());
    if(myRobot->getRobotEngine()!=NULL)
    {
        if(myRobot->getRobotEngine()->isOpen())
            myRobot->getRobotEngine()->sendCommand(STOP,0);
        else
            qDebug("Engine not connected");
    }

}
