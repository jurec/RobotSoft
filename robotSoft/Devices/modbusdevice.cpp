#include "modbusdevice.h"
#include <QIODevice>

modbusDevice::modbusDevice(QObject *parent) :
    QIODevice(parent)
{
}

modbusDevice::~modbusDevice()
{

}
QByteArray modbusDevice::read(const int &registerAddress, const int &dataSize)
{
    QByteArray rez;
    uint16_t data[dataSize];
    modbus_read_registers(modbus,registerAddress,dataSize,data);
    for(int i=0;i<dataSize;i++)
            rez.push_back(data[i]);
    return rez;
}

void modbusDevice::write(const int &registerAddress, const int &data)
{
    modbus_write_register(modbus,registerAddress,data);
}

bool modbusDevice::open()
{
  modbus=modbus_new_rtu(getComPort().toStdString().c_str(),getBaudRate(),getParity(),getDataBits(),getStopBits());
    modbus_set_slave(modbus,getDeviceAddress());
   return (! modbus_connect(modbus) );
}
bool modbusDevice::open(const char *device, int baud, char parity, int data_bit, int stop_bit)
{
    setComPort(device);
    setBaudRate(baud);
    setParity(parity);
    setDataBits(data_bit);
    setStopBits(stop_bit);
    return open();
    //modbus=modbus_new_rtu(getComPort().toStdString().c_str(),getBaudRate(),getParity(),getDataBits(),getStopBits());
     // modbus_set_slave(modbus,getDeviceAddress());
     //return (modbus_connect(modbus) ? false:  true);
}

void modbusDevice::close()
{
    modbus_close(modbus);
    modbus_free(modbus);
}
