#include <Devices/tong.h>
#include <iostream>
tong::tong()
{
}


void tong::open(){

    tongModbus=modbus_new_rtu("COM8",57600,'E',8,1);
    modbus_set_slave(tongModbus,address);
    if(modbus_connect(tongModbus)) std::cout<<"No connection";
}
void tong::close()
{
    modbus_close(tongModbus);
    modbus_free(tongModbus);
}

void tong::pickUp()
{
    modbus_write_register(tongModbus,commandRegisterAddress,7);    

}
void tong::putOff()
{
    modbus_write_register(tongModbus,commandRegisterAddress,DOWN_RELEASE);
}
QString tong::readCurrentCommand()
{
    uint16_t tab_reg[10];
      tab_reg[0]=0;
    modbus_read_registers(tongModbus,commandRegisterAddress,1,tab_reg);
   return static_cast<QString> (tab_reg[0]);
}
