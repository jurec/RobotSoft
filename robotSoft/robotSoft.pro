#-------------------------------------------------
#
# Project created by QtCreator 2011-04-26T14:45:35
#
#-------------------------------------------------

QT       += core

QT       += gui
QT       += network
TARGET = robotSoft
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Devices/tongdevice.cpp \
    Devices/tong.cpp \
    Devices/tcpsocketdevice.cpp \
    Devices/startstopdevice.cpp \
    Devices/modbusdevice.cpp \
    Devices/enginedevice.cpp \
    Devices/camera.cpp \
    Modbus/modbus-rtu.c \
    Modbus/modbus-data.c \
    Modbus/modbus.c \
    robotcommands.cpp \
    robot.cpp \
    Field/robotfield.cpp \
    Field/cell.cpp \
    Command/command.cpp \
    robotparameters.cpp

HEADERS += \
    Devices/tongdevice.h \
    Devices/tong.h \
    Devices/tcpsocketdevice.h \
    Devices/startstopdevice.h \
    Devices/modbusdevice.h \
    Devices/enginedevice.h \
    Devices/camera.h \
    Modbus/modbus-version.h \
    Modbus/modbus-tcp-private.h \
    Modbus/modbus-rtu-private.h \
    Modbus/modbus-rtu.h \
    Modbus/modbus-private.h \
    Modbus/modbus.h \
    Modbus/config.h \
    robotcommands.h \
    robot.h \
    Field/robotfield.h \
    Field/cell.h \
    Command/command.h \
    robotparameters.h
