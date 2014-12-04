/****************************************************************************
** Meta object code from reading C++ file 'vsccameraadd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../main/vsccameraadd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vsccameraadd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VSCCameraAdd_t {
    QByteArrayData data[6];
    char stringdata[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VSCCameraAdd_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VSCCameraAdd_t qt_meta_stringdata_VSCCameraAdd = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VSCCameraAdd"
QT_MOC_LITERAL(1, 13, 14), // "floatingAction"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "unFloatingAction"
QT_MOC_LITERAL(4, 46, 18), // "radioButtonClicked"
QT_MOC_LITERAL(5, 65, 11) // "applyConfig"

    },
    "VSCCameraAdd\0floatingAction\0\0"
    "unFloatingAction\0radioButtonClicked\0"
    "applyConfig"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VSCCameraAdd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VSCCameraAdd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VSCCameraAdd *_t = static_cast<VSCCameraAdd *>(_o);
        switch (_id) {
        case 0: _t->floatingAction(); break;
        case 1: _t->unFloatingAction(); break;
        case 2: _t->radioButtonClicked(); break;
        case 3: _t->applyConfig(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VSCCameraAdd::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VSCCameraAdd.data,
      qt_meta_data_VSCCameraAdd,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VSCCameraAdd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VSCCameraAdd::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VSCCameraAdd.stringdata))
        return static_cast<void*>(const_cast< VSCCameraAdd*>(this));
    return QWidget::qt_metacast(_clname);
}

int VSCCameraAdd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_END_MOC_NAMESPACE
