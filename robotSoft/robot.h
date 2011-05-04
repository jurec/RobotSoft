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

    void simple_action(int angle, int distance,bool isForwardDirection, bool isPick);

    void setRobotCenter(QPointF point){robotParameter->setRobotCenter(point);}
    robotParameters* getRobotParametrs(){return robotParameter;}
    //void setRobotParameters(robotParameters &param){robotParameter->operator =(param);}
    engineDevice* getRobotEngine(){return robotEngine;}
    tongDevice* getRobotTong(){return robotTong;}


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
