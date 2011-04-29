#include "enginedevice.h"
#include <QDebug>
engineDevice::engineDevice()
{
    command[0]=COMMAND_PREFIX;
}
void engineDevice::sendCommand(int com, int distance)
{
    command[1]=com;
    command[2]=distance/256;
    command[3]=distance%256;
    command[4]=500/256;
    command[5]=500%256;
    write(command);
  //  socket.waitForBytesWritten(200);
    //engineSocket.write(command);
    //engineSocket.waitForBytesWritten(200);
}
//void engineDevice::moveForward(int distance)
//{
//    sendCommand(FORWARD,distance);
//}
//void engineDevice::moveBackward(int distance)
//{
//    sendCommand(BACKWARD,distance);
//}
//void engineDevice::turn(int angle)
//{
//    sendCommand(TURN,angle);
//}
//void engineDevice::stop()
//{
//    sendCommand(STOP,255);
//}
