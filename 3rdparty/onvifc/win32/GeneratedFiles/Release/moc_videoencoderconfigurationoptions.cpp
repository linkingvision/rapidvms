/****************************************************************************
** Meta object code from reading C++ file 'videoencoderconfigurationoptions.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/media_management/videoencoderconfigurationoptions.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoencoderconfigurationoptions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions_t {
    QByteArrayData data[6];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions_t qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions = {
    {
QT_MOC_LITERAL(0, 0, 39), // "ONVIF::VideoEncoderConfigurat..."
QT_MOC_LITERAL(1, 40, 21), // "H264ProfilesSupported"
QT_MOC_LITERAL(2, 62, 8), // "Baseline"
QT_MOC_LITERAL(3, 71, 4), // "Main"
QT_MOC_LITERAL(4, 76, 8), // "Extended"
QT_MOC_LITERAL(5, 85, 4) // "High"

    },
    "ONVIF::VideoEncoderConfigurationOptions\0"
    "H264ProfilesSupported\0Baseline\0Main\0"
    "Extended\0High"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__VideoEncoderConfigurationOptions[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    4,   18,

 // enum data: key, value
       2, uint(ONVIF::VideoEncoderConfigurationOptions::Baseline),
       3, uint(ONVIF::VideoEncoderConfigurationOptions::Main),
       4, uint(ONVIF::VideoEncoderConfigurationOptions::Extended),
       5, uint(ONVIF::VideoEncoderConfigurationOptions::High),

       0        // eod
};

void ONVIF::VideoEncoderConfigurationOptions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ONVIF::VideoEncoderConfigurationOptions::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions.data,
      qt_meta_data_ONVIF__VideoEncoderConfigurationOptions,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ONVIF::VideoEncoderConfigurationOptions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::VideoEncoderConfigurationOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__VideoEncoderConfigurationOptions.stringdata))
        return static_cast<void*>(const_cast< VideoEncoderConfigurationOptions*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::VideoEncoderConfigurationOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
