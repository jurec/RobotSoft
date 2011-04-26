#include "camera.h"

camera::camera(QObject *parent) :
    tcpSocketDevice(parent)
{
    connect(this,SIGNAL(dataReady()),this,SLOT(dataProc()));
}
void camera::sendCommand(QString b)
{
    write(b.toStdString().c_str());
}
void camera::dataProc()
{
    //qDebug(QString::fromAscii(readAll()).toStdString().c_str());
    //clearBuffer();
}
QPair<int,int> camera::returnDistanceAngle()
{
    QPair<int,int> point;
    point.first=20;
    point.second=0;
    return point;
}
