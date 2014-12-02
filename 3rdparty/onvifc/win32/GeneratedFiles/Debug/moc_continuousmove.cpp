/****************************************************************************
** Meta object code from reading C++ file 'continuousmove.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ptz_management/continuousmove.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'continuousmove.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__ContinuousMove_t {
    QByteArrayData data[19];
    char stringdata[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__ContinuousMove_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__ContinuousMove_t qt_meta_stringdata_ONVIF__ContinuousMove = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ONVIF::ContinuousMove"
QT_MOC_LITERAL(1, 22, 15), // "setProfileToken"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 3), // "arg"
QT_MOC_LITERAL(4, 43, 11), // "setPanTiltX"
QT_MOC_LITERAL(5, 55, 11), // "setPanTiltY"
QT_MOC_LITERAL(6, 67, 15), // "setPanTiltSpace"
QT_MOC_LITERAL(7, 83, 8), // "setZoomX"
QT_MOC_LITERAL(8, 92, 12), // "setZoomSpace"
QT_MOC_LITERAL(9, 105, 9), // "setResult"
QT_MOC_LITERAL(10, 115, 10), // "setTimeout"
QT_MOC_LITERAL(11, 126, 12), // "profileToken"
QT_MOC_LITERAL(12, 139, 8), // "panTiltX"
QT_MOC_LITERAL(13, 148, 8), // "panTiltY"
QT_MOC_LITERAL(14, 157, 12), // "panTiltSpace"
QT_MOC_LITERAL(15, 170, 5), // "zoomX"
QT_MOC_LITERAL(16, 176, 9), // "zoomSpace"
QT_MOC_LITERAL(17, 186, 7), // "timeout"
QT_MOC_LITERAL(18, 194, 6) // "result"

    },
    "ONVIF::ContinuousMove\0setProfileToken\0"
    "\0arg\0setPanTiltX\0setPanTiltY\0"
    "setPanTiltSpace\0setZoomX\0setZoomSpace\0"
    "setResult\0setTimeout\0profileToken\0"
    "panTiltX\0panTiltY\0panTiltSpace\0zoomX\0"
    "zoomSpace\0timeout\0result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__ContinuousMove[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       8,   78, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    1,   57,    2, 0x0a /* Public */,
       5,    1,   60,    2, 0x0a /* Public */,
       6,    1,   63,    2, 0x0a /* Public */,
       7,    1,   66,    2, 0x0a /* Public */,
       8,    1,   69,    2, 0x0a /* Public */,
       9,    1,   72,    2, 0x0a /* Public */,
      10,    1,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // properties: name, type, flags
      11, QMetaType::QString, 0x00095103,
      12, QMetaType::Float, 0x00095103,
      13, QMetaType::Float, 0x00095103,
      14, QMetaType::QString, 0x00095103,
      15, QMetaType::Float, 0x00095103,
      16, QMetaType::QString, 0x00095103,
      17, QMetaType::QString, 0x00095103,
      18, QMetaType::Bool, 0x00095103,

       0        // eod
};

void ONVIF::ContinuousMove::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ContinuousMove *_t = static_cast<ContinuousMove *>(_o);
        switch (_id) {
        case 0: _t->setProfileToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setPanTiltX((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->setPanTiltY((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setPanTiltSpace((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setZoomX((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setZoomSpace((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setTimeout((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::ContinuousMove::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__ContinuousMove.data,
      qt_meta_data_ONVIF__ContinuousMove,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::ContinuousMove::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::ContinuousMove::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__ContinuousMove.stringdata))
        return static_cast<void*>(const_cast< ContinuousMove*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::ContinuousMove::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = profileToken(); break;
        case 1: *reinterpret_cast< float*>(_v) = panTiltX(); break;
        case 2: *reinterpret_cast< float*>(_v) = panTiltY(); break;
        case 3: *reinterpret_cast< QString*>(_v) = panTiltSpace(); break;
        case 4: *reinterpret_cast< float*>(_v) = zoomX(); break;
        case 5: *reinterpret_cast< QString*>(_v) = zoomSpace(); break;
        case 6: *reinterpret_cast< QString*>(_v) = timeout(); break;
        case 7: *reinterpret_cast< bool*>(_v) = result(); break;
        default: break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setProfileToken(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPanTiltX(*reinterpret_cast< float*>(_v)); break;
        case 2: setPanTiltY(*reinterpret_cast< float*>(_v)); break;
        case 3: setPanTiltSpace(*reinterpret_cast< QString*>(_v)); break;
        case 4: setZoomX(*reinterpret_cast< float*>(_v)); break;
        case 5: setZoomSpace(*reinterpret_cast< QString*>(_v)); break;
        case 6: setTimeout(*reinterpret_cast< QString*>(_v)); break;
        case 7: setResult(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
