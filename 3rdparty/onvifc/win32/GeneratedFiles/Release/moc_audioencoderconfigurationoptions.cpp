/****************************************************************************
** Meta object code from reading C++ file 'audioencoderconfigurationoptions.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/media_management/audioencoderconfigurationoptions.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audioencoderconfigurationoptions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions_t {
    QByteArrayData data[5];
    char stringdata[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions_t qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions = {
    {
QT_MOC_LITERAL(0, 0, 39), // "ONVIF::AudioEncoderConfigurat..."
QT_MOC_LITERAL(1, 40, 8), // "Encoding"
QT_MOC_LITERAL(2, 49, 4), // "G711"
QT_MOC_LITERAL(3, 54, 4), // "G726"
QT_MOC_LITERAL(4, 59, 3) // "AAC"

    },
    "ONVIF::AudioEncoderConfigurationOptions\0"
    "Encoding\0G711\0G726\0AAC"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__AudioEncoderConfigurationOptions[] = {

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
       1, 0x0,    3,   18,

 // enum data: key, value
       2, uint(ONVIF::AudioEncoderConfigurationOptions::G711),
       3, uint(ONVIF::AudioEncoderConfigurationOptions::G726),
       4, uint(ONVIF::AudioEncoderConfigurationOptions::AAC),

       0        // eod
};

void ONVIF::AudioEncoderConfigurationOptions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ONVIF::AudioEncoderConfigurationOptions::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions.data,
      qt_meta_data_ONVIF__AudioEncoderConfigurationOptions,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::AudioEncoderConfigurationOptions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::AudioEncoderConfigurationOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__AudioEncoderConfigurationOptions.stringdata))
        return static_cast<void*>(const_cast< AudioEncoderConfigurationOptions*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::AudioEncoderConfigurationOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
