#ifndef CAMERA_H
#define CAMERA_H
#include <Devices/tcpsocketdevice.h>
#include <QPair>
class camera : public tcpSocketDevice
{
    Q_OBJECT
public:
    explicit camera(QObject *parent = 0);
    void sendCommand(QString b);
    qint64 readData(char *data, qint64 maxlen){return 0;}
    qint64 writeData(const char *data, qint64 len){return 0;}
    QPair<int,int> returnDistanceAngle();//Возвращает расстояние до пешки и угол на котором она находится
signals:

public slots:
    void dataProc();
};

#endif // CAMERA_H
