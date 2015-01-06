/****************************************************************************
** Meta object code from reading C++ file 'qhttp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/qhttp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qhttp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QHttp_t {
    QByteArrayData data[36];
    char stringdata[526];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QHttp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QHttp_t qt_meta_stringdata_QHttp = {
    {
QT_MOC_LITERAL(0, 0, 5), // "QHttp"
QT_MOC_LITERAL(1, 6, 12), // "stateChanged"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 22), // "responseHeaderReceived"
QT_MOC_LITERAL(4, 43, 19), // "QHttpResponseHeader"
QT_MOC_LITERAL(5, 63, 4), // "resp"
QT_MOC_LITERAL(6, 68, 9), // "readyRead"
QT_MOC_LITERAL(7, 78, 16), // "dataSendProgress"
QT_MOC_LITERAL(8, 95, 16), // "dataReadProgress"
QT_MOC_LITERAL(9, 112, 14), // "requestStarted"
QT_MOC_LITERAL(10, 127, 15), // "requestFinished"
QT_MOC_LITERAL(11, 143, 4), // "done"
QT_MOC_LITERAL(12, 148, 27), // "proxyAuthenticationRequired"
QT_MOC_LITERAL(13, 176, 13), // "QNetworkProxy"
QT_MOC_LITERAL(14, 190, 5), // "proxy"
QT_MOC_LITERAL(15, 196, 15), // "QAuthenticator*"
QT_MOC_LITERAL(16, 212, 22), // "authenticationRequired"
QT_MOC_LITERAL(17, 235, 8), // "hostname"
QT_MOC_LITERAL(18, 244, 4), // "port"
QT_MOC_LITERAL(19, 249, 9), // "sslErrors"
QT_MOC_LITERAL(20, 259, 16), // "QList<QSslError>"
QT_MOC_LITERAL(21, 276, 6), // "errors"
QT_MOC_LITERAL(22, 283, 5), // "abort"
QT_MOC_LITERAL(23, 289, 15), // "ignoreSslErrors"
QT_MOC_LITERAL(24, 305, 19), // "_q_startNextRequest"
QT_MOC_LITERAL(25, 325, 16), // "_q_slotReadyRead"
QT_MOC_LITERAL(26, 342, 16), // "_q_slotConnected"
QT_MOC_LITERAL(27, 359, 12), // "_q_slotError"
QT_MOC_LITERAL(28, 372, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(29, 401, 13), // "_q_slotClosed"
QT_MOC_LITERAL(30, 415, 19), // "_q_slotBytesWritten"
QT_MOC_LITERAL(31, 435, 8), // "numBytes"
QT_MOC_LITERAL(32, 444, 28), // "_q_slotEncryptedBytesWritten"
QT_MOC_LITERAL(33, 473, 17), // "_q_slotDoFinished"
QT_MOC_LITERAL(34, 491, 18), // "_q_slotSendRequest"
QT_MOC_LITERAL(35, 510, 15) // "_q_continuePost"

    },
    "QHttp\0stateChanged\0\0responseHeaderReceived\0"
    "QHttpResponseHeader\0resp\0readyRead\0"
    "dataSendProgress\0dataReadProgress\0"
    "requestStarted\0requestFinished\0done\0"
    "proxyAuthenticationRequired\0QNetworkProxy\0"
    "proxy\0QAuthenticator*\0authenticationRequired\0"
    "hostname\0port\0sslErrors\0QList<QSslError>\0"
    "errors\0abort\0ignoreSslErrors\0"
    "_q_startNextRequest\0_q_slotReadyRead\0"
    "_q_slotConnected\0_q_slotError\0"
    "QAbstractSocket::SocketError\0_q_slotClosed\0"
    "_q_slotBytesWritten\0numBytes\0"
    "_q_slotEncryptedBytesWritten\0"
    "_q_slotDoFinished\0_q_slotSendRequest\0"
    "_q_continuePost"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QHttp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  129,    2, 0x06 /* Public */,
       3,    1,  132,    2, 0x06 /* Public */,
       6,    1,  135,    2, 0x06 /* Public */,
       7,    2,  138,    2, 0x06 /* Public */,
       8,    2,  143,    2, 0x06 /* Public */,
       9,    1,  148,    2, 0x06 /* Public */,
      10,    2,  151,    2, 0x06 /* Public */,
      11,    1,  156,    2, 0x06 /* Public */,
      12,    2,  159,    2, 0x06 /* Public */,
      16,    3,  164,    2, 0x06 /* Public */,
      19,    1,  171,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    0,  174,    2, 0x0a /* Public */,
      23,    0,  175,    2, 0x0a /* Public */,
      24,    0,  176,    2, 0x08 /* Private */,
      25,    0,  177,    2, 0x08 /* Private */,
      26,    0,  178,    2, 0x08 /* Private */,
      27,    1,  179,    2, 0x08 /* Private */,
      29,    0,  182,    2, 0x08 /* Private */,
      30,    1,  183,    2, 0x08 /* Private */,
      32,    1,  186,    2, 0x08 /* Private */,
      33,    0,  189,    2, 0x08 /* Private */,
      34,    0,  190,    2, 0x08 /* Private */,
      35,    0,  191,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, 0x80000000 | 15,   17,   18,    2,
    QMetaType::Void, 0x80000000 | 20,   21,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 28,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   31,
    QMetaType::Void, QMetaType::LongLong,   31,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QHttp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QHttp *_t = static_cast<QHttp *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->responseHeaderReceived((*reinterpret_cast< const QHttpResponseHeader(*)>(_a[1]))); break;
        case 2: _t->readyRead((*reinterpret_cast< const QHttpResponseHeader(*)>(_a[1]))); break;
        case 3: _t->dataSendProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->dataReadProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->requestStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->requestFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->done((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->proxyAuthenticationRequired((*reinterpret_cast< const QNetworkProxy(*)>(_a[1])),(*reinterpret_cast< QAuthenticator*(*)>(_a[2]))); break;
        case 9: _t->authenticationRequired((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< QAuthenticator*(*)>(_a[3]))); break;
        case 10: _t->sslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 11: _t->abort(); break;
        case 12: _t->ignoreSslErrors(); break;
        case 13: _t->d->_q_startNextRequest(); break;
        case 14: _t->d->_q_slotReadyRead(); break;
        case 15: _t->d->_q_slotConnected(); break;
        case 16: _t->d->_q_slotError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 17: _t->d->_q_slotClosed(); break;
        case 18: _t->d->_q_slotBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 19: _t->d->_q_slotEncryptedBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 20: _t->d->_q_slotDoFinished(); break;
        case 21: _t->d->_q_slotSendRequest(); break;
        case 22: _t->d->_q_continuePost(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QHttp::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::stateChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (QHttp::*_t)(const QHttpResponseHeader & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::responseHeaderReceived)) {
                *result = 1;
            }
        }
        {
            typedef void (QHttp::*_t)(const QHttpResponseHeader & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::readyRead)) {
                *result = 2;
            }
        }
        {
            typedef void (QHttp::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::dataSendProgress)) {
                *result = 3;
            }
        }
        {
            typedef void (QHttp::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::dataReadProgress)) {
                *result = 4;
            }
        }
        {
            typedef void (QHttp::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::requestStarted)) {
                *result = 5;
            }
        }
        {
            typedef void (QHttp::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::requestFinished)) {
                *result = 6;
            }
        }
        {
            typedef void (QHttp::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::done)) {
                *result = 7;
            }
        }
        {
            typedef void (QHttp::*_t)(const QNetworkProxy & , QAuthenticator * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::proxyAuthenticationRequired)) {
                *result = 8;
            }
        }
        {
            typedef void (QHttp::*_t)(const QString & , quint16 , QAuthenticator * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::authenticationRequired)) {
                *result = 9;
            }
        }
        {
            typedef void (QHttp::*_t)(const QList<QSslError> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QHttp::sslErrors)) {
                *result = 10;
            }
        }
    }
}

const QMetaObject QHttp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QHttp.data,
      qt_meta_data_QHttp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QHttp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QHttp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QHttp.stringdata))
        return static_cast<void*>(const_cast< QHttp*>(this));
    return QObject::qt_metacast(_clname);
}

int QHttp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void QHttp::stateChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QHttp::responseHeaderReceived(const QHttpResponseHeader & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QHttp::readyRead(const QHttpResponseHeader & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QHttp::dataSendProgress(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QHttp::dataReadProgress(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QHttp::requestStarted(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QHttp::requestFinished(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QHttp::done(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QHttp::proxyAuthenticationRequired(const QNetworkProxy & _t1, QAuthenticator * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QHttp::authenticationRequired(const QString & _t1, quint16 _t2, QAuthenticator * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QHttp::sslErrors(const QList<QSslError> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
