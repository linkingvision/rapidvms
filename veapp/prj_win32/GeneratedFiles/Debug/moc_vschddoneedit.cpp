/****************************************************************************
** Meta object code from reading C++ file 'vschddoneedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../disk/vschddoneedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vschddoneedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VSCHddOneEdit_t {
    QByteArrayData data[5];
    char stringdata[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VSCHddOneEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VSCHddOneEdit_t qt_meta_stringdata_VSCHddOneEdit = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VSCHddOneEdit"
QT_MOC_LITERAL(1, 14, 12), // "setFreeValue"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "value"
QT_MOC_LITERAL(4, 34, 12) // "diskSelected"

    },
    "VSCHddOneEdit\0setFreeValue\0\0value\0"
    "diskSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VSCHddOneEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void VSCHddOneEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VSCHddOneEdit *_t = static_cast<VSCHddOneEdit *>(_o);
        switch (_id) {
        case 0: _t->setFreeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->diskSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VSCHddOneEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VSCHddOneEdit.data,
      qt_meta_data_VSCHddOneEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VSCHddOneEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VSCHddOneEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VSCHddOneEdit.stringdata))
        return static_cast<void*>(const_cast< VSCHddOneEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int VSCHddOneEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
