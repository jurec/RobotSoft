#ifndef MODBUSDEVICE_H
#define MODBUSDEVICE_H
#include <QIODevice>
#include <Modbus/modbus.h>
class modbusDevice : public QIODevice
{
    Q_OBJECT
public:
    explicit modbusDevice(QObject *parent = 0);
    int getBaudRate()const {return baudRate;}
    int getStopBits() const {return stopBits;}
    char getParity() const {return parity;}
    int getDataBits() const {return dataBits;}
    QString getComPort()const{return comPort;}
    int getDeviceAddress()const{return deviceAddress;}

    void setBaudRate(const int &baudRate_){baudRate=baudRate_;}
    void setParity(const char &parity_){parity=parity_;}
    void setStopBits(const int &stopBits_){stopBits=stopBits_;}
    void setDataBits(const int &dataBits_){dataBits=dataBits_;}
    void setDeviceAddress(const int &deviceAddress_){deviceAddress=deviceAddress_;}
    void setComPort(QString comPort_){comPort=comPort_;}
    virtual ~modbusDevice();
    bool isOpen(){return openStatus;}
    bool open(const char *device,
              int baud, char parity, int data_bit,
              int stop_bit);
    bool open();
    void close();
    void write(const int &registerAddress,const int &data);
    QByteArray read(const int &registerAddress,const int &dataSize);
    enum PARITY_VAL{parity_even='E',parity_odd='O',parity_none='N'};
    static const int startAddress=1200;
private:

    modbus_t* modbus;
    bool openStatus;
    int deviceAddress;
    QString comPort;
    int baudRate;
    int stopBits;
    int dataBits;
    char parity;

signals:

public slots:

};

#endif // MODBUSDEVICE_H
