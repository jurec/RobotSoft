#ifndef TONG_H
#define TONG_H
#include <QIODevice>
#include <Modbus/modbus.h>
class tong
{
public:
    tong();
   // open(OpenMode mode);
    void open();
    void close();
    void pickUp();
    void putOff();

    bool isConnect();
  //  void command1();
   // bool isOpen();
    QString readCurrentCommand();
private:
    modbus_t* tongModbus;
    static const int address=0x0A;

   // bool portIsOpen;
   // ManageSerialPort tongPort;

    enum {commandRegisterAddress=1200, errorRegisterAddress,
          temperatureRegister1, temperatureRegister2, temperatureRegister3, temperatureRegister4,
          torqueRegister1,torqueRegister2,torqueRegister3,torqueRegister4 };
    enum {BEGIN_STATE=1,
          READY_CATCH,
          CATCH_PAWN,
          PAWN_UP,
          PAWN_DOWN,
          RELEASE,
          CATCH_UP,
          DOWN_RELEASE,
          STOP=10,
          START};
};

#endif // TONG_H
