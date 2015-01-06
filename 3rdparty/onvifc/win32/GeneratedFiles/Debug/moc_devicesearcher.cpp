/****************************************************************************
** Meta object code from reading C++ file 'devicesearcher.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/devicesearcher.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicesearcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__DeviceSearcher_t {
    QByteArrayData data[6];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__DeviceSearcher_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__DeviceSearcher_t qt_meta_stringdata_ONVIF__DeviceSearcher = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ONVIF::DeviceSearcher"
QT_MOC_LITERAL(1, 22, 11), // "receiveData"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 22), // "QHash<QString,QString>"
QT_MOC_LITERAL(4, 58, 4), // "data"
QT_MOC_LITERAL(5, 63, 20) // "readPendingDatagrams"

    },
    "ONVIF::DeviceSearcher\0receiveData\0\0"
    "QHash<QString,QString>\0data\0"
    "readPendingDatagrams"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__DeviceSearcher[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   27,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ONVIF::DeviceSearcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceSearcher *_t = static_cast<DeviceSearcher *>(_o);
        switch (_id) {
        case 0: _t->receiveData((*reinterpret_cast< const QHash<QString,QString>(*)>(_a[1]))); break;
        case 1: _t->readPendingDatagrams(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DeviceSearcher::*_t)(const QHash<QString,QString> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DeviceSearcher::receiveData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ONVIF::DeviceSearcher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__DeviceSearcher.data,
      qt_meta_data_ONVIF__DeviceSearcher,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ONVIF::DeviceSearcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::DeviceSearcher::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__DeviceSearcher.stringdata))
        return static_cast<void*>(const_cast< DeviceSearcher*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::DeviceSearcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}

// SIGNAL 0
void ONVIF::DeviceSearcher::receiveData(const QHash<QString,QString> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
