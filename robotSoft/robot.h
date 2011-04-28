#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QSettings>
#include <Devices/enginedevice.h>
#include <Devices/modbusdevice.h>
#include <Devices/tongdevice.h>
#include <Devices/camera.h>
#include <QRect>
#include <QPoint>
#include <Devices/startstopdevice.h>
#include <qmath.h>
#include <robotparameters.h>
class robot : public QObject
{
    Q_OBJECT
public:
    explicit robot(QObject *parent = 0);
    void action();
    ~robot();
   // enum teamColors{RED=0,BLUE=1};
    //Команды которые может выполнять робот
        //команды схвата
    void pickUp();//Схватить пешку и
    void putOff();//Опустить пешку на клетку
    void putOnPion();//Опустить пешку на пешку
    void torqueOff();//Снять нагрузку
        //команды движения
    void moveForward(int distance);//Ехать вперед
    void moveBackward(int distance);//Ехать назад
    void turn(int angle);//угол поворота(-влево, + вправо)
    void stopRobot();//Остановиться

        //команды получения и обработки информации с камеры и устройств

        //определение цели и маршрута движения

    void simple_action(int angle, int distance,bool isForwardDirection, bool isPick);

    QString getTongState();

  //  void setXCoordinate(int coord){xCoordinate=coord;}
   // void setYCoordinate(int coord){yCoordinate=coord;}
    //void changeRobotCoordinate(int x,int y){robotCoordinate.x+=x;robotCoordinate.y+=y;}

    void setAngle(int angle_){angle=angle_;}
    void setWidth(int width_){robotParameter->robotModel.setWidth(width_);}
    void setLength(int length_){robotParameter->robotModel.setHeight(length_);}
    void setRobotCenter(){
        robotParameter->robotCenter.setX(getWidth()/2);
        robotParameter->robotCenter.setY(getLength()/2);}
    void setRed(){robotParameter->isRed=true;}
    void setBlue(){robotParameter->isRed=false;}
    void setRobotCenter(QPointF point){robotParameter->robotCenter=point;}
    //QRect getRobot(){return QRect(0,0,25,35);}
   // QPoint getRobotCoordinate(){return robotCoordinate;}
  //  int getXCoordinate(){return xCoordinate;}
  //  int getYCoordinate(){return yCoordinate;}
    int getAngle(){return angle;}
    int getWidth(){return robotParameter->robotModel.width();}
    int getLength(){return robotParameter->robotModel.height();}
    QPointF getRobotCenter(){return robotParameter->robotCenter;}
    robotParameters* getRobotParametrs(){return robotParameter;}
    void init_devices();
    void readSettings();
    void writeSettings();

    QPointF robotCoordinate;//Координаты центра робота

signals:
   // void readSettingsError(QString);
public slots:

    void displayData();
    void stop();//Запускается по истечении времени
    void start();//Запускает программу
private:
  //  teamColors robotColor;
    QSettings *sett;
    robotParameters *robotParameter;
    int angle;
    startStopDevice ssSystem;
    camera robotCamera;
    engineDevice robotEngine;
    tongDevice robotTong;
    //  engineControlSocket engine;
    //tong robotTong;   
};

#endif // ROBOT_H
