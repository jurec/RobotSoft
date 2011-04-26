#ifndef TCPSOCKETDEVICE_H
#define TCPSOCKETDEVICE_H

#include <QIODevice>
#include <QTcpSocket>
class tcpSocketDevice : public QIODevice
{
    Q_OBJECT
public:
    explicit tcpSocketDevice(QObject *parent = 0);
    bool open();
    bool open(QString address,int portName);
    void close();

    int getPortName()const{return portName;}
    void setPortName(int portName_){portName=portName_;}

    void setSocketAddress(QString hostAddress_){hostAddress=hostAddress_;}
    QString getSocketAddress()const{return hostAddress;}

    void write(const char *data);
    QByteArray read();
    void clearBuffer();

signals:
   void dataReady();
public slots:
    void dataArrived();
private:
    QTcpSocket socket;
    QString hostAddress;
    int portName;

};

#endif // TCPSOCKETDEVICE_H
