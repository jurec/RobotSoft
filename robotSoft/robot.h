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

    void setAngle(int angle_){angle=angle_;}
    void setWidth(int width_){robotParameter->robotModel.setWidth(width_);}
    void setLength(int length_){robotParameter->robotModel.setHeight(length_);}
    void setRed(){robotParameter->isRed=true;}
    void setBlue(){robotParameter->isRed=false;}
    void setRobotCenter(QPointF point){robotParameter->robotCenter=point;}
    int getAngle(){return angle;}
    int getWidth(){return robotParameter->robotModel.width();}
    int getLength(){return robotParameter->robotModel.height();}
    QPointF getRobotCenter(){return robotParameter->robotCenter;}
    robotParameters* getRobotParametrs(){return robotParameter;}
    engineDevice* getRobotEngine(){return robotEngine;}
 //  tongDevice* getRobotTong(){return *robotTong;}

    void init_devices(); //������������� ���������
    void readSettings();//������ �������� �� �����
    void writeSettings();//������ �������� � ����

signals:
   // void readSettingsError(QString);
public slots:
    void displayData();
    void stop();//����������� �� ��������� �������
    void start();//��������� ���������
private:
  //  teamColors robotColor;
    QSettings *sett;
    robotParameters *robotParameter;
    int angle;
    startStopDevice ssSystem;
    camera *robotCamera;
    engineDevice *robotEngine;
    tongDevice *robotTong;
   // engineDevice *ed;
    //  engineControlSocket engine;
    //tong robotTong;   
};

#endif // ROBOT_H
