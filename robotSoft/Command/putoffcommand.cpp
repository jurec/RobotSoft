#include "putoffcommand.h"

putOffCommand::putOffCommand(QObject *parent) :
    command(parent)
{
    setName("PUT OFF");
}
void putOffCommand::execute(robot *myRobot)
{
    //�������� ������
    setName(getName());
    if(myRobot->getRobotTong()!=NULL)
    {
        if(myRobot->getRobotTong()->open())
                myRobot->getRobotTong()->sendCommand(DOWN_RELEASE);
        else
            qDebug("Engine not connected");
    }
    //������ ���� �� ����������� ���������� ������ ������
}
