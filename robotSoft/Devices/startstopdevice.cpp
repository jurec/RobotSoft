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
    //Действия в ожидании стартового сигнала
    //Считывание с линии
    emit {start1(); }
    //После посылки сигнала начиннает работать таймер
}
void startStopDevice::stopDrivers()
{
    //Здесь посылается сигнал об отключении питания у двигателей
    qDebug("Drivers stopped");
}

void startStopDevice::sendStop()
{
    //Когда таймер остановился вызывается этот обработчик
    timer.stop();
    emit {stop();}
}
int startStopDevice::getTime()
{
   return time.elapsed();
}
void startStopDevice::startTime()
{
    //Начинается отсчет времени
    time.start();
}
