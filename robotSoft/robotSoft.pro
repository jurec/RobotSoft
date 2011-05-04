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
INCLUDEPATH += c:\boost\boost\graph

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
    robot.cpp \
    Field/robotfield.cpp \
    Field/cell.cpp \
    Command/command.cpp \
    robotparameters.cpp \
    Command/moveforwardcommand.cpp \
    taskrunner.cpp \
    Command/movebackwardcommand.cpp \
    Command/rotatecommand.cpp \
    Command/stopcommand.cpp \
    Command/pickupcommand.cpp \
    Command/putoffcommand.cpp \
    Command/putonpioncommand.cpp

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
    robot.h \
    Field/robotfield.h \
    Field/cell.h \
    Command/command.h \
    robotparameters.h \
    Command/moveforwardcommand.h \
    taskrunner.h \
    Command/movebackwardcommand.h \
    Command/rotatecommand.h \
    Command/stopcommand.h \
    Command/pickupcommand.h \
    Command/putoffcommand.h \
    Command/putonpioncommand.h
