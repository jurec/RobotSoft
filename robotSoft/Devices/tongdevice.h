#ifndef TONGDEVICE_H
#define TONGDEVICE_H
#include <Devices/modbusdevice.h>
class tongDevice : public modbusDevice
{
public:
    tongDevice();
    tongDevice(int address);
    //enum commands {FORWARD=1,BACKWARD,LEFT,RIGHT};

    qint64 readData(char *data, qint64 maxlen){return 0;}
    qint64 writeData(const char *data, qint64 len){return 0;}
    enum {commandRegisterAddress=1200, errorRegisterAddress,
          temperatureRegister1, temperatureRegister2, temperatureRegister3, temperatureRegister4,
          torqueRegister1,torqueRegister2,torqueRegister3,torqueRegister4 };
    enum commands {BEGIN_STATE=1,
          READY_CATCH,
          CATCH_PAWN,
          PAWN_UP,
          PAWN_DOWN,
          RELEASE,
          CATCH_UP,
          DOWN_RELEASE,
          STOP=10,
          START};
    bool init();
     void sendCommand(int com);
private:

};

#endif // TONGDEVICE_H
