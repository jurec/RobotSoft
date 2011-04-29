#ifndef ROBOTPARAMETERS_H
#define ROBOTPARAMETERS_H
//Класс служит для передачи парметров робота
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
    QRect robotModel;//Тут храняться начальные координаты робота его размеры(длина, ширина)
    bool isRed;//хранится цвет команды 0-синий 1-красный
    QPointF robotCenter;//хранятся координаты центра робота, относительно которых он разворачивается
signals:

public slots:   
};

#endif // ROBOTPARAMETERS_H
