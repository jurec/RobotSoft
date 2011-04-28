/****************************************************************************
** Meta object code from reading C++ file 'robotfield.h'
**
** Created: Thu 28. Apr 13:54:50 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../robotSoft/Field/robotfield.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'robotfield.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_robotField[] = {

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
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_robotField[] = {
    "robotField\0\0mousePressed()\0drawRobot()\0"
};

const QMetaObject robotField::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_robotField,
      qt_meta_data_robotField, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &robotField::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *robotField::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *robotField::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_robotField))
        return static_cast<void*>(const_cast< robotField*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int robotField::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mousePressed(); break;
        case 1: drawRobot(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void robotField::mousePressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
