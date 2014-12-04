/****************************************************************************
** Meta object code from reading C++ file 'vscdevicetrash.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../main/vscdevicetrash.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vscdevicetrash.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VSCDeviceTrash_t {
    QByteArrayData data[3];
    char stringdata[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VSCDeviceTrash_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VSCDeviceTrash_t qt_meta_stringdata_VSCDeviceTrash = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VSCDeviceTrash"
QT_MOC_LITERAL(1, 15, 13), // "CameraDeleted"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "VSCDeviceTrash\0CameraDeleted\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VSCDeviceTrash[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void VSCDeviceTrash::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VSCDeviceTrash *_t = static_cast<VSCDeviceTrash *>(_o);
        switch (_id) {
        case 0: _t->CameraDeleted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VSCDeviceTrash::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VSCDeviceTrash::CameraDeleted)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VSCDeviceTrash::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_VSCDeviceTrash.data,
      qt_meta_data_VSCDeviceTrash,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VSCDeviceTrash::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VSCDeviceTrash::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VSCDeviceTrash.stringdata))
        return static_cast<void*>(const_cast< VSCDeviceTrash*>(this));
    return QPushButton::qt_metacast(_clname);
}

int VSCDeviceTrash::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void VSCDeviceTrash::CameraDeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
