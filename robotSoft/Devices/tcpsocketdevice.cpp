#include "tcpsocketdevice.h"

tcpSocketDevice::tcpSocketDevice(QObject *parent) :
    QIODevice(parent)
{
    connect(&socket,SIGNAL(readyRead()),this,SLOT(dataArrived()));
}
void tcpSocketDevice::dataArrived()
{
    emit dataReady();
}
void tcpSocketDevice::clearBuffer()
{
    socket.flush();
}

bool tcpSocketDevice::open()
{
    socket.connectToHost(getSocketAddress(),getPortName());
    socket.waitForConnected(50);
    return socket.isOpen();
}
bool tcpSocketDevice::open(QString address, int portName)
{
    setSocketAddress(address);
    setPortName(portName);
    return open();
}

void tcpSocketDevice::close()
{
    socket.disconnectFromHost();
    socket.waitForDisconnected(50);
}
void tcpSocketDevice::write(const char *data)
{
    socket.flush();
    socket.write(data);
    socket.waitForBytesWritten(50);
}

QByteArray tcpSocketDevice::read()
{
    socket.waitForReadyRead(50);
    QByteArray myArr(socket.readAll());
    //socket.readyRead();
    socket.flush();
    return myArr;
}
