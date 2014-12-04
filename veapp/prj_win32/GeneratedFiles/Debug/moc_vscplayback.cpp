/****************************************************************************
** Meta object code from reading C++ file 'vscplayback.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../main/vscplayback.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vscplayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VSCPlayback_t {
    QByteArrayData data[12];
    char stringdata[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VSCPlayback_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VSCPlayback_t qt_meta_stringdata_VSCPlayback = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VSCPlayback"
QT_MOC_LITERAL(1, 12, 15), // "floatingClicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "TabbedClicked"
QT_MOC_LITERAL(4, 43, 18), // "ShowDisplayClicked"
QT_MOC_LITERAL(5, 62, 3), // "nId"
QT_MOC_LITERAL(6, 66, 16), // "ShowFocusClicked"
QT_MOC_LITERAL(7, 83, 15), // "ShowPlayControl"
QT_MOC_LITERAL(8, 99, 18), // "ShowLayout1Clicked"
QT_MOC_LITERAL(9, 118, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(10, 133, 12), // "QMouseEvent*"
QT_MOC_LITERAL(11, 146, 1) // "e"

    },
    "VSCPlayback\0floatingClicked\0\0TabbedClicked\0"
    "ShowDisplayClicked\0nId\0ShowFocusClicked\0"
    "ShowPlayControl\0ShowLayout1Clicked\0"
    "mouseMoveEvent\0QMouseEvent*\0e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VSCPlayback[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    1,   51,    2, 0x0a /* Public */,
       6,    1,   54,    2, 0x0a /* Public */,
       7,    0,   57,    2, 0x0a /* Public */,
       8,    1,   58,    2, 0x0a /* Public */,
       9,    1,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void VSCPlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VSCPlayback *_t = static_cast<VSCPlayback *>(_o);
        switch (_id) {
        case 0: _t->floatingClicked(); break;
        case 1: _t->TabbedClicked(); break;
        case 2: _t->ShowDisplayClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ShowFocusClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ShowPlayControl(); break;
        case 5: _t->ShowLayout1Clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VSCPlayback::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VSCPlayback.data,
      qt_meta_data_VSCPlayback,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VSCPlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VSCPlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VSCPlayback.stringdata))
        return static_cast<void*>(const_cast< VSCPlayback*>(this));
    return QWidget::qt_metacast(_clname);
}

int VSCPlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
