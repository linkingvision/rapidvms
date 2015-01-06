/****************************************************************************
** Meta object code from reading C++ file 'stop.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ptz_management/stop.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__Stop_t {
    QByteArrayData data[11];
    char stringdata[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__Stop_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__Stop_t qt_meta_stringdata_ONVIF__Stop = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ONVIF::Stop"
QT_MOC_LITERAL(1, 12, 15), // "setProfileToken"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "arg"
QT_MOC_LITERAL(4, 33, 10), // "setPanTilt"
QT_MOC_LITERAL(5, 44, 7), // "setZoom"
QT_MOC_LITERAL(6, 52, 9), // "setResult"
QT_MOC_LITERAL(7, 62, 12), // "profileToken"
QT_MOC_LITERAL(8, 75, 7), // "panTilt"
QT_MOC_LITERAL(9, 83, 4), // "zoom"
QT_MOC_LITERAL(10, 88, 6) // "result"

    },
    "ONVIF::Stop\0setProfileToken\0\0arg\0"
    "setPanTilt\0setZoom\0setResult\0profileToken\0"
    "panTilt\0zoom\0result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__Stop[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       5,    1,   40,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // properties: name, type, flags
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::Bool, 0x00095103,
       9, QMetaType::Bool, 0x00095103,
      10, QMetaType::Bool, 0x00095103,

       0        // eod
};

void ONVIF::Stop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Stop *_t = static_cast<Stop *>(_o);
        switch (_id) {
        case 0: _t->setProfileToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setPanTilt((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setZoom((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::Stop::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__Stop.data,
      qt_meta_data_ONVIF__Stop,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ONVIF::Stop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::Stop::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__Stop.stringdata))
        return static_cast<void*>(const_cast< Stop*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::Stop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = profileToken(); break;
        case 1: *reinterpret_cast< bool*>(_v) = panTilt(); break;
        case 2: *reinterpret_cast< bool*>(_v) = zoom(); break;
        case 3: *reinterpret_cast< bool*>(_v) = result(); break;
        default: break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setProfileToken(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPanTilt(*reinterpret_cast< bool*>(_v)); break;
        case 2: setZoom(*reinterpret_cast< bool*>(_v)); break;
        case 3: setResult(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
