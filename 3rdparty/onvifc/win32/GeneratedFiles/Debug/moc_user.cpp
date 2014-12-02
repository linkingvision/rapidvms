/****************************************************************************
** Meta object code from reading C++ file 'user.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/device_management/user.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'user.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__Users_t {
    QByteArrayData data[15];
    char stringdata[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__Users_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__Users_t qt_meta_stringdata_ONVIF__Users = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ONVIF::Users"
QT_MOC_LITERAL(1, 13, 11), // "setUserName"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "arg"
QT_MOC_LITERAL(4, 30, 11), // "setPassWord"
QT_MOC_LITERAL(5, 42, 12), // "setUserLevel"
QT_MOC_LITERAL(6, 55, 13), // "UserLevelType"
QT_MOC_LITERAL(7, 69, 8), // "userName"
QT_MOC_LITERAL(8, 78, 8), // "passWord"
QT_MOC_LITERAL(9, 87, 9), // "userLevel"
QT_MOC_LITERAL(10, 97, 13), // "Administrator"
QT_MOC_LITERAL(11, 111, 8), // "Operator"
QT_MOC_LITERAL(12, 120, 4), // "User"
QT_MOC_LITERAL(13, 125, 9), // "Anonymous"
QT_MOC_LITERAL(14, 135, 8) // "Extended"

    },
    "ONVIF::Users\0setUserName\0\0arg\0setPassWord\0"
    "setUserLevel\0UserLevelType\0userName\0"
    "passWord\0userLevel\0Administrator\0"
    "Operator\0User\0Anonymous\0Extended"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__Users[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   38, // properties
       1,   47, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       5,    1,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 6,    3,

 // properties: name, type, flags
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::QString, 0x00095103,
       9, 0x80000000 | 6, 0x0009510b,

 // enums: name, flags, count, data
       6, 0x0,    5,   51,

 // enum data: key, value
      10, uint(ONVIF::Users::Administrator),
      11, uint(ONVIF::Users::Operator),
      12, uint(ONVIF::Users::User),
      13, uint(ONVIF::Users::Anonymous),
      14, uint(ONVIF::Users::Extended),

       0        // eod
};

void ONVIF::Users::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Users *_t = static_cast<Users *>(_o);
        switch (_id) {
        case 0: _t->setUserName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setPassWord((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setUserLevel((*reinterpret_cast< UserLevelType(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::Users::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__Users.data,
      qt_meta_data_ONVIF__Users,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::Users::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::Users::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__Users.stringdata))
        return static_cast<void*>(const_cast< Users*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::Users::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = userName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = passWord(); break;
        case 2: *reinterpret_cast< UserLevelType*>(_v) = userLevel(); break;
        default: break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setUserName(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPassWord(*reinterpret_cast< QString*>(_v)); break;
        case 2: setUserLevel(*reinterpret_cast< UserLevelType*>(_v)); break;
        default: break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
