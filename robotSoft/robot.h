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
class robot : public QObject
{
    Q_OBJECT
public:
    explicit robot(QObject *parent = 0);
    void action();
    ~robot();
    enum teamColors{RED=0,BLUE=1};
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
    void setWidth(int width_){width=width;}
    void setLength(int length_){length=length_;}

    QRect getRobot(){return QRect(0,0,25,35);}
   // QPoint getRobotCoordinate(){return robotCoordinate;}
  //  int getXCoordinate(){return xCoordinate;}
  //  int getYCoordinate(){return yCoordinate;}
    int getAngle(){return angle;}
    int getWidth(){return width;}
    int getLength(){return length;}

    void init_devices();
    void readSettings();
    void writeSettings();

    QPoint robotCoordinate;//Координаты центра робота

signals:
   // void readSettingsError(QString);
public slots:
 //   void showReadSettingsError();
    void displayData();
    void stop();//Запускается по истечении времени
    void start();//Запускает программу
private:
    teamColors robotColor;
    QSettings *sett;
  //  int xCoordinate;
  //  int yCoordinate;
    int width;
    int length;
    int angle;
    startStopDevice ssSystem;
    camera robotCamera;
    engineDevice robotEngine;
    tongDevice robotTong;
    //  engineControlSocket engine;
    //tong robotTong;   
};

#endif // ROBOT_H
