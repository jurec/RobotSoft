#include "robot.h"
#include <QSettings>
#include <iostream>
#include <QtGlobal>
robot::robot(QObject *parent) :
    QObject(parent)
{
//    connect(&robotCamera,SIGNAL(dataReady()),this,SLOT(displayData()));
//    sett=new QSettings("settings.ini",QSettings::IniFormat);
//    connect(&ssSystem,SIGNAL(start1()),this,SLOT(start()));
//    connect(&ssSystem,SIGNAL(stop()),this,SLOT(stop()));
    robotParameter=new robotParameters;
}
void robot::displayData()
{
    qDebug("some data arrived");
    robotCamera->readAll();
    robotCamera->clearBuffer();
    //qDebug();
}

void robot::init_devices()
{
/***************Инициализирует устройства робота
  Создает новые устройства
Считывает натсройки каждого из устройств  из файла командой readSettings
Проверяет все ли устройства готовы к работе
Если нет выдает сообщение об ошибке
Если все устроцства готовы, ждет стартового сигнала от системы стоп-остановка(ssSystem)******************/
robotEngine = new engineDevice();
robotTong = new tongDevice();
robotCamera = new camera();
//readSettings();
//if(!robotCamera.open()) qDebug("Cannot connect to Camera");
// std::cout<<robotTong.getDeviceAddress()<<" "<<robotTong.getComPort().toStdString();
//  if(!robotTong.open())   qDebug("Tong not open");
//if(!robotEngine.open()) qDebug("Cannot connect to Engine");

//ssSystem.waitForStart(); Чтобы запустить раскомментировать
}
void robot::readSettings()
{
 sett=new QSettings("settings.ini",QSettings::IniFormat);

 sett->beginGroup("Timer");
    ssSystem.setTimerInterval(sett->value("Time").toInt());
 sett->endGroup();

 sett->beginGroup("Tong");
    robotTong->setDeviceAddress(sett->value("deviceAddress").toInt());
    robotTong->setComPort(sett->value("comPort").toString());
    robotTong->setBaudRate(sett->value("baudRate").toInt());
    robotTong->setStopBits(sett->value("stopBits").toInt());
    robotTong->setParity((sett->value("parity").toInt()));
 sett->endGroup();

 sett->beginGroup("StartStopSystem");
    ssSystem.setDeviceAddress(sett->value("deviceAddress").toInt());
    ssSystem.setComPort(sett->value("comPort").toString());
    ssSystem.setBaudRate(sett->value("baudRate").toInt());
    ssSystem.setStopBits(sett->value("stopBits").toInt());
    ssSystem.setParity((sett->value("parity").toInt()));
 sett->endGroup();

 sett->beginGroup("EngineSocket");
    robotEngine->setSocketAddress(sett->value("hostAddress").toString());
    robotEngine->setPortName(sett->value("portName").toInt());
 sett->endGroup();

 sett->beginGroup("cameraSocket");
    robotCamera->setSocketAddress(sett->value("hostAddress").toString());
    robotCamera->setPortName(sett->value("portName").toInt());
 sett->endGroup();

 sett->beginGroup("RobotDimensions");
  // setWidth(sett->value("width",35).toInt());
  // setLength(sett->value("length",25).toInt());
sett->endGroup();
delete sett;
}

void robot::writeSettings()
{
    sett=new QSettings("settings.ini",QSettings::IniFormat);
    sett->beginGroup("Timer");
      //  sett->setValue("Time",90000);
        sett->setValue("Time",90000);
    sett->endGroup();

//    sett->beginGroup("EngineLeft");
//        sett->setValue("deviceAddress",0x00);
//        sett->setValue("comPort","COM3");
//        sett->setValue("baudRate",57600);
//        sett->setValue("stopBits",1);
//        sett->setValue("parity",'E');
//    sett->endGroup();

//    sett->beginGroup("EngineRight");
//        sett->setValue("deviceAddress",0x00);
//        sett->setValue("comPort","COM3");
//        sett->setValue("baudRate",57600);
//        sett->setValue("stopBits",1);
//        sett->setValue("parity",'E');
//    sett->endGroup();

    sett->beginGroup("Tong");
        sett->setValue("deviceAddress",robotTong->getDeviceAddress());
        sett->setValue("comPort",robotTong->getComPort());
        sett->setValue("baudRate",robotTong->getBaudRate());
        sett->setValue("stopBits",robotTong->getStopBits());
        sett->setValue("parity",'E');
    sett->endGroup();

    sett->beginGroup("StartStopSystem");
        sett->setValue("deviceAddress",ssSystem.getDeviceAddress());
        sett->setValue("comPort",ssSystem.getComPort());
        sett->setValue("baudRate",ssSystem.getBaudRate());
        sett->setValue("stopBits",ssSystem.getStopBits());
        sett->setValue("parity",'N');
    sett->endGroup();

//    sett->beginGroup("Tong");
//        sett->setValue("deviceAddress",0x0A);
//        sett->setValue("comPort","COM8");
//        sett->setValue("baudRate",57600);
//        sett->setValue("stopBits",1);
//        sett->setValue("parity",'E');
//    sett->endGroup();

    sett->beginGroup("EngineSocket");
    sett->setValue("hostAddress",robotEngine->getSocketAddress());
    sett->setValue("portName",robotEngine->getPortName());
//        sett->setValue("hostAddress","127.0.0.1");
//        sett->setValue("portName",39999);
    sett->endGroup();

    sett->beginGroup("cameraSocket");
//        sett->setValue("hostAddress","172.16.2.59");
//        sett->setValue("portName",9050);
        sett->setValue("hostAddress",robotCamera->getSocketAddress());
        sett->setValue("portName",robotCamera->getPortName());
    sett->endGroup();

    sett->beginGroup("RobotDimensions");
     //   sett->setValue("width",getWidth());
      //  sett->setValue("length",getLength());
    sett->endGroup();
    delete sett;
}

robot::~robot()
{
   // robotEngine.close();
}
void robot::start()
{
    qDebug("Start !!! Time %d", ssSystem.getTime());
}
void robot::stop()
{
    ssSystem.stopDrivers();
    qDebug("Stop !!! Time %d", ssSystem.getTime());
}

void robot::simple_action(int angle, int distance,bool isForwardDirection, bool isPick)
{
    //turn(angle);
  // torqueOff();
}
/*Команды схвата*/
//void robot::torqueOff()
//{
//  //  robotTong.sendCommand(robotCommands::TORQUE_OFF);
//}
//void robot::pickUp()
//{
//   // robotTong.sendCommand(robotCommands::CATCH_UP);
//}
//void robot::putOff()
//{
////    robotTong.sendCommand(robotCommands::DOWN_RELEASE);

//}
//void robot::putOnPion()
//{
//   // robotTong.sendCommand(robotCommands::DOWN_RELEASE);
//}
/*Конец командам схвата*/

/*Команды двигателя*/
//void robot::turn(int angle)
//{
//   // setAngle(getAngle()+angle);
//  //  robotEngine.sendCommand(robotCommands::TURN,angle);
//}

//void robot::moveBackward(int distance)
//{
//    //robotCoordinate+=QPoint(qSin(getAngle())*distance, qCos(getAngle())*distance);
//   // robotEngine.sendCommand(robotCommands::BACKWARD,distance);
//}
//void robot::moveForward(int distance)
//{
//    //robotCoordinate-=QPoint(qSin(getAngle())*distance, qCos(getAngle())*distance);
//    //robotEngine.sendCommand(robotCommands::FORWARD,distance);
//}
//void robot::stopRobot()
//{
//  //  robotEngine.sendCommand(robotCommands::STOP,255);
//}
