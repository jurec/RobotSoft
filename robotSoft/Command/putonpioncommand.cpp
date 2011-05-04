#include "putonpioncommand.h"

putOnPionCommand::putOnPionCommand(QObject *parent) :
    command(parent)
{
    setName ("PUT ON PION");
}
void putOnPionCommand::execute(robot *myRobot)
{
    //Движение вперед
    setName(getName());
    if(myRobot->getRobotTong()!=NULL)
    {
        if(myRobot->getRobotTong()->open())
                myRobot->getRobotTong()->sendCommand(DOWN_ON_PION);
        else
            qDebug("Engine not connected");
    }
    //Дальше надо бы пересчитать координаты центра робота
}
