/****************************************************************************
** Meta object code from reading C++ file 'vscsearch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../main/vscsearch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vscsearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VSCSearch_t {
    QByteArrayData data[18];
    char stringdata[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VSCSearch_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VSCSearch_t qt_meta_stringdata_VSCSearch = {
    {
QT_MOC_LITERAL(0, 0, 9), // "VSCSearch"
QT_MOC_LITERAL(1, 10, 15), // "NewSearchedItem"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "astring"
QT_MOC_LITERAL(4, 35, 2), // "IP"
QT_MOC_LITERAL(5, 38, 4), // "Port"
QT_MOC_LITERAL(6, 43, 12), // "Manufacturer"
QT_MOC_LITERAL(7, 56, 5), // "Model"
QT_MOC_LITERAL(8, 62, 15), // "FirmwareVersion"
QT_MOC_LITERAL(9, 78, 12), // "ONVIFAddress"
QT_MOC_LITERAL(10, 91, 11), // "StartSearch"
QT_MOC_LITERAL(11, 103, 10), // "StopSearch"
QT_MOC_LITERAL(12, 114, 6), // "AddAll"
QT_MOC_LITERAL(13, 121, 9), // "SelectAll"
QT_MOC_LITERAL(14, 131, 7), // "AddItem"
QT_MOC_LITERAL(15, 139, 17), // "SearchReceiveData"
QT_MOC_LITERAL(16, 157, 22), // "QHash<QString,QString>"
QT_MOC_LITERAL(17, 180, 4) // "data"

    },
    "VSCSearch\0NewSearchedItem\0\0astring\0"
    "IP\0Port\0Manufacturer\0Model\0FirmwareVersion\0"
    "ONVIFAddress\0StartSearch\0StopSearch\0"
    "AddAll\0SelectAll\0AddItem\0SearchReceiveData\0"
    "QHash<QString,QString>\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VSCSearch[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    6,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   62,    2, 0x0a /* Public */,
      11,    0,   63,    2, 0x0a /* Public */,
      12,    0,   64,    2, 0x0a /* Public */,
      13,    0,   65,    2, 0x0a /* Public */,
      14,    6,   66,    2, 0x0a /* Public */,
      15,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3,    4,    5,    6,    7,    8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3,    4,    5,    6,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void VSCSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VSCSearch *_t = static_cast<VSCSearch *>(_o);
        switch (_id) {
        case 0: _t->NewSearchedItem((*reinterpret_cast< astring(*)>(_a[1])),(*reinterpret_cast< astring(*)>(_a[2])),(*reinterpret_cast< astring(*)>(_a[3])),(*reinterpret_cast< astring(*)>(_a[4])),(*reinterpret_cast< astring(*)>(_a[5])),(*reinterpret_cast< astring(*)>(_a[6]))); break;
        case 1: _t->StartSearch(); break;
        case 2: _t->StopSearch(); break;
        case 3: _t->AddAll(); break;
        case 4: _t->SelectAll(); break;
        case 5: _t->AddItem((*reinterpret_cast< astring(*)>(_a[1])),(*reinterpret_cast< astring(*)>(_a[2])),(*reinterpret_cast< astring(*)>(_a[3])),(*reinterpret_cast< astring(*)>(_a[4])),(*reinterpret_cast< astring(*)>(_a[5])),(*reinterpret_cast< astring(*)>(_a[6]))); break;
        case 6: _t->SearchReceiveData((*reinterpret_cast< const QHash<QString,QString>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VSCSearch::*_t)(astring , astring , astring , astring , astring , astring );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VSCSearch::NewSearchedItem)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject VSCSearch::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VSCSearch.data,
      qt_meta_data_VSCSearch,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VSCSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VSCSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VSCSearch.stringdata))
        return static_cast<void*>(const_cast< VSCSearch*>(this));
    return QWidget::qt_metacast(_clname);
}

int VSCSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void VSCSearch::NewSearchedItem(astring _t1, astring _t2, astring _t3, astring _t4, astring _t5, astring _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
