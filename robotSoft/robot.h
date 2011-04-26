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
    //������� ������� ����� ��������� �����
        //������� ������
    void pickUp();//�������� ����� �
    void putOff();//�������� ����� �� ������
    void putOnPion();//�������� ����� �� �����
    void torqueOff();//����� ��������
        //������� ��������
    void moveForward(int distance);//����� ������
    void moveBackward(int distance);//����� �����
    void turn(int angle);//���� ��������(-�����, + ������)
    void stopRobot();//������������

        //������� ��������� � ��������� ���������� � ������ � ���������

        //����������� ���� � �������� ��������



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

    QPoint robotCoordinate;//���������� ������ ������

signals:
   // void readSettingsError(QString);
public slots:
 //   void showReadSettingsError();
    void displayData();
    void stop();//����������� �� ��������� �������
    void start();//��������� ���������
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
