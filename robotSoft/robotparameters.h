#ifndef ROBOTPARAMETERS_H
#define ROBOTPARAMETERS_H
//����� ������ ��� �������� ��������� ������
#include <QObject>
#include <QRect>
class robotParameters : public QObject
{
    Q_OBJECT
public:
   explicit robotParameters(QObject *parent = 0);
//    void setWidth(int w){robotModel->setWidth(w);}
//    void setHeight(int h){robotModel->setHeight(h);}
//    void setTeamRed(){isRed=true;}
//    void setTeamBlue(){isRed=false;}
//    void setRobotModel(QRect &robotMod){*robotModel=robotMod;}
    QRect robotModel;//��� ��������� ��������� ���������� ������ ��� �������(�����, ������)
    bool isRed;//�������� ���� ������� 0-����� 1-�������
    QPointF robotCenter;//�������� ���������� ������ ������, ������������ ������� �� ���������������
signals:

public slots:   
};

#endif // ROBOTPARAMETERS_H
