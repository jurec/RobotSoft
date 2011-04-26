#include "tongdevice.h"
tongDevice::tongDevice()
{
    setDeviceAddress(0x0A);
}

tongDevice::tongDevice(int address)
{
    setDeviceAddress(address);
}
bool tongDevice::init()
{
    return open("COM5",57600,'E',8,1);

}
void tongDevice::sendCommand(int com)
{
    write(1200,com);
}
