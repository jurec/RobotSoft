/****************************************************************************
** Meta object code from reading C++ file 'tcpsocketdevice.h'
**
** Created: Tue 26. Apr 15:44:15 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../robotSoft/Devices/tcpsocketdevice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpsocketdevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcpSocketDevice[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tcpSocketDevice[] = {
    "tcpSocketDevice\0\0dataReady()\0dataArrived()\0"
};

const QMetaObject tcpSocketDevice::staticMetaObject = {
    { &QIODevice::staticMetaObject, qt_meta_stringdata_tcpSocketDevice,
      qt_meta_data_tcpSocketDevice, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcpSocketDevice::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcpSocketDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcpSocketDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcpSocketDevice))
        return static_cast<void*>(const_cast< tcpSocketDevice*>(this));
    return QIODevice::qt_metacast(_clname);
}

int tcpSocketDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QIODevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dataReady(); break;
        case 1: dataArrived(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void tcpSocketDevice::dataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
