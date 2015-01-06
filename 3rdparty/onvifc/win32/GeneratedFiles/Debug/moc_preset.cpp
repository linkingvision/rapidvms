/****************************************************************************
** Meta object code from reading C++ file 'preset.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ptz_management/preset.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'preset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__Preset_t {
    QByteArrayData data[11];
    char stringdata[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__Preset_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__Preset_t qt_meta_stringdata_ONVIF__Preset = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ONVIF::Preset"
QT_MOC_LITERAL(1, 14, 15), // "setProfileToken"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "arg"
QT_MOC_LITERAL(4, 35, 13), // "setPresetName"
QT_MOC_LITERAL(5, 49, 14), // "setPresetToken"
QT_MOC_LITERAL(6, 64, 9), // "setResult"
QT_MOC_LITERAL(7, 74, 12), // "profileToken"
QT_MOC_LITERAL(8, 87, 10), // "presetName"
QT_MOC_LITERAL(9, 98, 11), // "presetToken"
QT_MOC_LITERAL(10, 110, 6) // "result"

    },
    "ONVIF::Preset\0setProfileToken\0\0arg\0"
    "setPresetName\0setPresetToken\0setResult\0"
    "profileToken\0presetName\0presetToken\0"
    "result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__Preset[] = {

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
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // properties: name, type, flags
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::QString, 0x00095103,
       9, QMetaType::QString, 0x00095103,
      10, QMetaType::Bool, 0x00095103,

       0        // eod
};

void ONVIF::Preset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Preset *_t = static_cast<Preset *>(_o);
        switch (_id) {
        case 0: _t->setProfileToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setPresetName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setPresetToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::Preset::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__Preset.data,
      qt_meta_data_ONVIF__Preset,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ONVIF::Preset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::Preset::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__Preset.stringdata))
        return static_cast<void*>(const_cast< Preset*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::Preset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 1: *reinterpret_cast< QString*>(_v) = presetName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = presetToken(); break;
        case 3: *reinterpret_cast< bool*>(_v) = result(); break;
        default: break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setProfileToken(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPresetName(*reinterpret_cast< QString*>(_v)); break;
        case 2: setPresetToken(*reinterpret_cast< QString*>(_v)); break;
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
