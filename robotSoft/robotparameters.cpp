#include "robotparameters.h"

robotParameters::robotParameters(QObject *parent) :
    QObject(parent)
{
    angle=0;
}
void robotParameters::rotateRobot(int angle)
{
    this->angle+=angle;
    robotCenterMatrix.rotate(angle);
    robotModelMatrix.rotate(angle);
}
void robotParameters::moveRobot(int distance)
{
    robotCenterMatrix.translate(qCos(getAngle())*distance,qSin(getAngle())*distance);
    robotModelMatrix.translate(qCos(getAngle())*distance,qSin(getAngle())*distance);
}
