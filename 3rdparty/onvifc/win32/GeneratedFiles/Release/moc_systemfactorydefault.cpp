/****************************************************************************
** Meta object code from reading C++ file 'systemfactorydefault.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/device_management/systemfactorydefault.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemfactorydefault.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__SystemFactoryDefault_t {
    QByteArrayData data[10];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__SystemFactoryDefault_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__SystemFactoryDefault_t qt_meta_stringdata_ONVIF__SystemFactoryDefault = {
    {
QT_MOC_LITERAL(0, 0, 27), // "ONVIF::SystemFactoryDefault"
QT_MOC_LITERAL(1, 28, 17), // "setFactoryDefault"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 14), // "FactoryDefault"
QT_MOC_LITERAL(4, 62, 3), // "arg"
QT_MOC_LITERAL(5, 66, 9), // "setResult"
QT_MOC_LITERAL(6, 76, 14), // "factoryDefault"
QT_MOC_LITERAL(7, 91, 6), // "result"
QT_MOC_LITERAL(8, 98, 4), // "Hard"
QT_MOC_LITERAL(9, 103, 4) // "Soft"

    },
    "ONVIF::SystemFactoryDefault\0"
    "setFactoryDefault\0\0FactoryDefault\0arg\0"
    "setResult\0factoryDefault\0result\0Hard\0"
    "Soft"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__SystemFactoryDefault[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   30, // properties
       1,   36, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       5,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    4,

 // properties: name, type, flags
       6, 0x80000000 | 3, 0x0009510b,
       7, QMetaType::Bool, 0x00095103,

 // enums: name, flags, count, data
       3, 0x0,    2,   40,

 // enum data: key, value
       8, uint(ONVIF::SystemFactoryDefault::Hard),
       9, uint(ONVIF::SystemFactoryDefault::Soft),

       0        // eod
};

void ONVIF::SystemFactoryDefault::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SystemFactoryDefault *_t = static_cast<SystemFactoryDefault *>(_o);
        switch (_id) {
        case 0: _t->setFactoryDefault((*reinterpret_cast< FactoryDefault(*)>(_a[1]))); break;
        case 1: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::SystemFactoryDefault::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__SystemFactoryDefault.data,
      qt_meta_data_ONVIF__SystemFactoryDefault,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::SystemFactoryDefault::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::SystemFactoryDefault::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__SystemFactoryDefault.stringdata))
        return static_cast<void*>(const_cast< SystemFactoryDefault*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::SystemFactoryDefault::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< FactoryDefault*>(_v) = factoryDefault(); break;
        case 1: *reinterpret_cast< bool*>(_v) = result(); break;
        default: break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFactoryDefault(*reinterpret_cast< FactoryDefault*>(_v)); break;
        case 1: setResult(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
