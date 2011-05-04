#include "taskrunner.h"

taskRunner::taskRunner(QObject *parent) :
    QObject(parent)
{
}
void taskRunner::runTasks(robot *myRobot)
{
    while(!tasks.isEmpty())
    {
        qDebug()<< tasks.front()->getName();
        tasks.front()->execute(myRobot);
        tasks.dequeue();
    }
}
