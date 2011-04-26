#ifndef STARTSTOPDEVICE_H
#define STARTSTOPDEVICE_H
#include <Devices/modbusdevice.h>
#include <QTimer>
#include <QTime>
class startStopDevice : public modbusDevice
{
    Q_OBJECT
public:
    explicit startStopDevice(QObject *parent = 0);
    void waitForStart();

    int getTime();
    void stopDrivers();
    void setTimerInterval(int interval){timerInterval=interval;}
    qint64 readData(char *data, qint64 maxlen){return 0;}
    qint64 writeData(const char *data, qint64 len){return 0;}
signals:
    void start1();
    void stop();

public slots:
    void sendStop();
    void startTime();
private:
    int timerInterval;
    QTimer timer;
    QTime time;
};

#endif // STARTSTOPDEVICE_H
