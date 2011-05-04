#ifndef ROBOTPARAMETERS_H
#define ROBOTPARAMETERS_H
//����� ������ ��� �������� ��������� ������
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QMatrix>
#include <qmath.h>
class robotParameters : public QObject
{
    Q_OBJECT
public:
    explicit robotParameters(QObject *parent = 0);
    void setRobotRect(QRectF rect){robotModel=rect; robotModelMatrix.mapRect(robotModel);}
    void setRobotCenter(QPointF center){robotCenter=center; robotCenterMatrix.map(robotCenter);}

    void setWidth(int w){robotModel.setWidth(w);}
    void setHeight(int h){robotModel.setHeight(h);}
    void setAngle(int a){angle=a;}
    QRectF getRobotModel(){return robotModel;}
    QPointF getRobotCenter(){return robotCenter;}
    QMatrix getRobotCenterMatrix(){return robotCenterMatrix;}
    QMatrix getRobotModelMatrix(){return robotModelMatrix;}

    void setTeamRed(){red=true;}
    void setTeamBlue(){red=false;}
    bool isRed(){return red;}
    int getAngle(){return angle;}
    void rotateRobot(int angle); //������� �������� ������� ���� � ��������� �������

    void moveRobot(int distance);

signals:

public slots:   
private:
    int angle;//���� �������
    QRectF robotModel;//��� ��������� ��������� ���������� ������ ��� �������(�����, ������)
    bool red;//�������� ���� ������� 0-����� 1-�������
    QPointF robotCenter;//�������� ���������� ������ ������, ������������ ������� �� ���������������
    QMatrix robotCenterMatrix;//������� �������� ��� ������ � ��� ����������� �����
    QMatrix robotModelMatrix;//���������������� � ������������
};

#endif // ROBOTPARAMETERS_H
