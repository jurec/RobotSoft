#include "pickupcommand.h"

pickUpCommand::pickUpCommand(QObject *parent) :
    command(parent)
{
    setName("PICK UP");
}
void pickUpCommand::execute(robot *myRobot)
{
    //Движение вперед
    setName(getName());
    if(myRobot->getRobotTong()!=NULL)
    {
        if(myRobot->getRobotTong()->open())
                myRobot->getRobotTong()->sendCommand(CATCH_UP);
        else
            qDebug("Engine not connected");
    }
    //Дальше надо бы пересчитать координаты центра робота
}
