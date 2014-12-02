/****************************************************************************
** Meta object code from reading C++ file 'systemdateandtime.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/device_management/systemdateandtime.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemdateandtime.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__SystemDateAndTime_t {
    QByteArrayData data[18];
    char stringdata[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__SystemDateAndTime_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__SystemDateAndTime_t qt_meta_stringdata_ONVIF__SystemDateAndTime = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ONVIF::SystemDateAndTime"
QT_MOC_LITERAL(1, 25, 18), // "setDaylightSavings"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 3), // "arg"
QT_MOC_LITERAL(4, 49, 15), // "setDateTimeType"
QT_MOC_LITERAL(5, 65, 12), // "DateTimeType"
QT_MOC_LITERAL(6, 78, 10), // "setutcTime"
QT_MOC_LITERAL(7, 89, 12), // "setlocalTime"
QT_MOC_LITERAL(8, 102, 5), // "setTz"
QT_MOC_LITERAL(9, 108, 9), // "setResult"
QT_MOC_LITERAL(10, 118, 12), // "dateTimeType"
QT_MOC_LITERAL(11, 131, 15), // "daylightSavings"
QT_MOC_LITERAL(12, 147, 2), // "tz"
QT_MOC_LITERAL(13, 150, 7), // "utcTime"
QT_MOC_LITERAL(14, 158, 9), // "localTime"
QT_MOC_LITERAL(15, 168, 6), // "result"
QT_MOC_LITERAL(16, 175, 6), // "Manual"
QT_MOC_LITERAL(17, 182, 3) // "NTP"

    },
    "ONVIF::SystemDateAndTime\0setDaylightSavings\0"
    "\0arg\0setDateTimeType\0DateTimeType\0"
    "setutcTime\0setlocalTime\0setTz\0setResult\0"
    "dateTimeType\0daylightSavings\0tz\0utcTime\0"
    "localTime\0result\0Manual\0NTP"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__SystemDateAndTime[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       6,   62, // properties
       1,   80, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       6,    1,   50,    2, 0x0a /* Public */,
       7,    1,   53,    2, 0x0a /* Public */,
       8,    1,   56,    2, 0x0a /* Public */,
       9,    1,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    3,
    QMetaType::Void, QMetaType::QDateTime,    3,
    QMetaType::Void, QMetaType::QDateTime,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // properties: name, type, flags
      10, 0x80000000 | 5, 0x0009510b,
      11, QMetaType::Bool, 0x00095103,
      12, QMetaType::QString, 0x00095103,
      13, QMetaType::QDateTime, 0x00095003,
      14, QMetaType::QDateTime, 0x00095003,
      15, QMetaType::Bool, 0x00095103,

 // enums: name, flags, count, data
       5, 0x0,    2,   84,

 // enum data: key, value
      16, uint(ONVIF::SystemDateAndTime::Manual),
      17, uint(ONVIF::SystemDateAndTime::NTP),

       0        // eod
};

void ONVIF::SystemDateAndTime::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SystemDateAndTime *_t = static_cast<SystemDateAndTime *>(_o);
        switch (_id) {
        case 0: _t->setDaylightSavings((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setDateTimeType((*reinterpret_cast< DateTimeType(*)>(_a[1]))); break;
        case 2: _t->setutcTime((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        case 3: _t->setlocalTime((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        case 4: _t->setTz((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::SystemDateAndTime::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__SystemDateAndTime.data,
      qt_meta_data_ONVIF__SystemDateAndTime,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::SystemDateAndTime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::SystemDateAndTime::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__SystemDateAndTime.stringdata))
        return static_cast<void*>(const_cast< SystemDateAndTime*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::SystemDateAndTime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< DateTimeType*>(_v) = dateTimeType(); break;
        case 1: *reinterpret_cast< bool*>(_v) = daylightSavings(); break;
        case 2: *reinterpret_cast< QString*>(_v) = tz(); break;
        case 3: *reinterpret_cast< QDateTime*>(_v) = utcTime(); break;
        case 4: *reinterpret_cast< QDateTime*>(_v) = localTime(); break;
        case 5: *reinterpret_cast< bool*>(_v) = result(); break;
        default: break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDateTimeType(*reinterpret_cast< DateTimeType*>(_v)); break;
        case 1: setDaylightSavings(*reinterpret_cast< bool*>(_v)); break;
        case 2: setTz(*reinterpret_cast< QString*>(_v)); break;
        case 3: setutcTime(*reinterpret_cast< QDateTime*>(_v)); break;
        case 4: setlocalTime(*reinterpret_cast< QDateTime*>(_v)); break;
        case 5: setResult(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
