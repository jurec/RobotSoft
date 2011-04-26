#ifndef ENGINEDEVICE_H
#define ENGINEDEVICE_H
#include <Devices/tcpsocketdevice.h>
#include <robotcommands.h>
class engineDevice : public tcpSocketDevice
{
public:
    engineDevice();
  //  enum commands {FORWARD=1,BACKWARD,LEFT,RIGHT,TURN,STOP};
    void sendCommand(int com,int distance);
//    void moveForward(int distance);
//    void moveBackward(int distance);
//    void turn(int angle);
//    void stop();
    qint64 readData(char *data, qint64 maxlen){return 0;}
    qint64 writeData(const char *data, qint64 len){return 0;}
private:

    static const int COMMAND_PREFIX=9;
    QByteArray command;
};

#endif // ENGINEDEVICE_H
