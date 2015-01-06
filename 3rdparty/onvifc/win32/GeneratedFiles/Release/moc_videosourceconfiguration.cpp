/****************************************************************************
** Meta object code from reading C++ file 'videosourceconfiguration.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/media_management/videosourceconfiguration.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videosourceconfiguration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__VideoSourceConfiguration_t {
    QByteArrayData data[13];
    char stringdata[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__VideoSourceConfiguration_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__VideoSourceConfiguration_t qt_meta_stringdata_ONVIF__VideoSourceConfiguration = {
    {
QT_MOC_LITERAL(0, 0, 31), // "ONVIF::VideoSourceConfiguration"
QT_MOC_LITERAL(1, 32, 8), // "setToken"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 3), // "arg"
QT_MOC_LITERAL(4, 46, 7), // "setName"
QT_MOC_LITERAL(5, 54, 11), // "setUseCount"
QT_MOC_LITERAL(6, 66, 14), // "setSourceToken"
QT_MOC_LITERAL(7, 81, 9), // "setBounds"
QT_MOC_LITERAL(8, 91, 5), // "token"
QT_MOC_LITERAL(9, 97, 4), // "name"
QT_MOC_LITERAL(10, 102, 8), // "useCount"
QT_MOC_LITERAL(11, 111, 11), // "sourceToken"
QT_MOC_LITERAL(12, 123, 6) // "bounds"

    },
    "ONVIF::VideoSourceConfiguration\0"
    "setToken\0\0arg\0setName\0setUseCount\0"
    "setSourceToken\0setBounds\0token\0name\0"
    "useCount\0sourceToken\0bounds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__VideoSourceConfiguration[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       5,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       5,    1,   45,    2, 0x0a /* Public */,
       6,    1,   48,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QRect,    3,

 // properties: name, type, flags
       8, QMetaType::QString, 0x00095103,
       9, QMetaType::QString, 0x00095103,
      10, QMetaType::Int, 0x00095103,
      11, QMetaType::QString, 0x00095103,
      12, QMetaType::QRect, 0x00095103,

       0        // eod
};

void ONVIF::VideoSourceConfiguration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoSourceConfiguration *_t = static_cast<VideoSourceConfiguration *>(_o);
        switch (_id) {
        case 0: _t->setToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setUseCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setSourceToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setBounds((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::VideoSourceConfiguration::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__VideoSourceConfiguration.data,
      qt_meta_data_ONVIF__VideoSourceConfiguration,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ONVIF::VideoSourceConfiguration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::VideoSourceConfiguration::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__VideoSourceConfiguration.stringdata))
        return static_cast<void*>(const_cast< VideoSourceConfiguration*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::VideoSourceConfiguration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = token(); break;
        case 1: *reinterpret_cast< QString*>(_v) = name(); break;
        case 2: *reinterpret_cast< int*>(_v) = useCount(); break;
        case 3: *reinterpret_cast< QString*>(_v) = sourceToken(); break;
        case 4: *reinterpret_cast< QRect*>(_v) = bounds(); break;
        default: break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setToken(*reinterpret_cast< QString*>(_v)); break;
        case 1: setName(*reinterpret_cast< QString*>(_v)); break;
        case 2: setUseCount(*reinterpret_cast< int*>(_v)); break;
        case 3: setSourceToken(*reinterpret_cast< QString*>(_v)); break;
        case 4: setBounds(*reinterpret_cast< QRect*>(_v)); break;
        default: break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
