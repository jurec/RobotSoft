#include "startstopdevice.h"

startStopDevice::startStopDevice(QObject *parent) :
    modbusDevice(parent)
{

    connect(this,SIGNAL(start1()),&timer,SLOT(start()));
    connect(this,SIGNAL(start1()),SLOT(startTime()));
    connect(&timer,SIGNAL(timeout()),SLOT(sendStop()));
}
void startStopDevice::waitForStart()
{
    timer.setInterval(timerInterval);
    //�������� � �������� ���������� �������
    //���������� � �����
    emit {start1(); }
    //����� ������� ������� ��������� �������� ������
}
void startStopDevice::stopDrivers()
{
    //����� ���������� ������ �� ���������� ������� � ����������
    qDebug("Drivers stopped");
}

void startStopDevice::sendStop()
{
    //����� ������ ����������� ���������� ���� ����������
    timer.stop();
    emit {stop();}
}
int startStopDevice::getTime()
{
   return time.elapsed();
}
void startStopDevice::startTime()
{
    //���������� ������ �������
    time.start();
}
