/****************************************************************************
** Meta object code from reading C++ file 'networkinterfaces.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/device_management/networkinterfaces.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkinterfaces.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__NetworkInterfaces_t {
    QByteArrayData data[38];
    char stringdata[585];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__NetworkInterfaces_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__NetworkInterfaces_t qt_meta_stringdata_ONVIF__NetworkInterfaces = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ONVIF::NetworkInterfaces"
QT_MOC_LITERAL(1, 25, 24), // "setNetworkInfacesEnabled"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 3), // "arg"
QT_MOC_LITERAL(4, 55, 18), // "setAutoNegotiation"
QT_MOC_LITERAL(5, 74, 8), // "setSpeed"
QT_MOC_LITERAL(6, 83, 9), // "setDuplex"
QT_MOC_LITERAL(7, 93, 6), // "Duplex"
QT_MOC_LITERAL(8, 100, 6), // "setMtu"
QT_MOC_LITERAL(9, 107, 14), // "setIpv4Enabled"
QT_MOC_LITERAL(10, 122, 20), // "setIpv4ManualAddress"
QT_MOC_LITERAL(11, 143, 25), // "setIpv4ManualPrefixLength"
QT_MOC_LITERAL(12, 169, 21), // "setNetworkInfacesName"
QT_MOC_LITERAL(13, 191, 13), // "setHwAaddress"
QT_MOC_LITERAL(14, 205, 23), // "setIpv4LinkLocalAddress"
QT_MOC_LITERAL(15, 229, 27), // "setIpvLinkLocalPrefixLength"
QT_MOC_LITERAL(16, 257, 22), // "setIpv4FromDHCPAddress"
QT_MOC_LITERAL(17, 280, 27), // "setIpv4FromDHCPPrefixLength"
QT_MOC_LITERAL(18, 308, 11), // "setIpv4DHCP"
QT_MOC_LITERAL(19, 320, 9), // "setResult"
QT_MOC_LITERAL(20, 330, 6), // "result"
QT_MOC_LITERAL(21, 337, 21), // "networkInfacesEnabled"
QT_MOC_LITERAL(22, 359, 15), // "autoNegotiation"
QT_MOC_LITERAL(23, 375, 5), // "speed"
QT_MOC_LITERAL(24, 381, 6), // "duplex"
QT_MOC_LITERAL(25, 388, 3), // "mtu"
QT_MOC_LITERAL(26, 392, 11), // "ipv4Enabled"
QT_MOC_LITERAL(27, 404, 17), // "ipv4ManualAddress"
QT_MOC_LITERAL(28, 422, 22), // "ipv4ManualPrefixLength"
QT_MOC_LITERAL(29, 445, 8), // "ipv4DHCP"
QT_MOC_LITERAL(30, 454, 18), // "networkInfacesName"
QT_MOC_LITERAL(31, 473, 10), // "hwAaddress"
QT_MOC_LITERAL(32, 484, 20), // "ipv4LinkLocalAddress"
QT_MOC_LITERAL(33, 505, 24), // "ipvLinkLocalPrefixLength"
QT_MOC_LITERAL(34, 530, 19), // "ipv4FromDHCPAddress"
QT_MOC_LITERAL(35, 550, 24), // "ipv4FromDHCPPrefixLength"
QT_MOC_LITERAL(36, 575, 4), // "Full"
QT_MOC_LITERAL(37, 580, 4) // "Half"

    },
    "ONVIF::NetworkInterfaces\0"
    "setNetworkInfacesEnabled\0\0arg\0"
    "setAutoNegotiation\0setSpeed\0setDuplex\0"
    "Duplex\0setMtu\0setIpv4Enabled\0"
    "setIpv4ManualAddress\0setIpv4ManualPrefixLength\0"
    "setNetworkInfacesName\0setHwAaddress\0"
    "setIpv4LinkLocalAddress\0"
    "setIpvLinkLocalPrefixLength\0"
    "setIpv4FromDHCPAddress\0"
    "setIpv4FromDHCPPrefixLength\0setIpv4DHCP\0"
    "setResult\0result\0networkInfacesEnabled\0"
    "autoNegotiation\0speed\0duplex\0mtu\0"
    "ipv4Enabled\0ipv4ManualAddress\0"
    "ipv4ManualPrefixLength\0ipv4DHCP\0"
    "networkInfacesName\0hwAaddress\0"
    "ipv4LinkLocalAddress\0ipvLinkLocalPrefixLength\0"
    "ipv4FromDHCPAddress\0ipv4FromDHCPPrefixLength\0"
    "Full\0Half"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__NetworkInterfaces[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
      16,  142, // properties
       1,  190, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x0a /* Public */,
       4,    1,   97,    2, 0x0a /* Public */,
       5,    1,  100,    2, 0x0a /* Public */,
       6,    1,  103,    2, 0x0a /* Public */,
       8,    1,  106,    2, 0x0a /* Public */,
       9,    1,  109,    2, 0x0a /* Public */,
      10,    1,  112,    2, 0x0a /* Public */,
      11,    1,  115,    2, 0x0a /* Public */,
      12,    1,  118,    2, 0x0a /* Public */,
      13,    1,  121,    2, 0x0a /* Public */,
      14,    1,  124,    2, 0x0a /* Public */,
      15,    1,  127,    2, 0x0a /* Public */,
      16,    1,  130,    2, 0x0a /* Public */,
      17,    1,  133,    2, 0x0a /* Public */,
      18,    1,  136,    2, 0x0a /* Public */,
      19,    1,  139,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 7,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // properties: name, type, flags
      20, QMetaType::Bool, 0x00095103,
      21, QMetaType::Bool, 0x00095103,
      22, QMetaType::Bool, 0x00095103,
      23, QMetaType::Int, 0x00095103,
      24, 0x80000000 | 7, 0x0009510b,
      25, QMetaType::Int, 0x00095103,
      26, QMetaType::Bool, 0x00095103,
      27, QMetaType::QString, 0x00095103,
      28, QMetaType::Int, 0x00095103,
      29, QMetaType::Bool, 0x00095103,
      30, QMetaType::QString, 0x00095103,
      31, QMetaType::QString, 0x00095103,
      32, QMetaType::QString, 0x00095103,
      33, QMetaType::Int, 0x00095103,
      34, QMetaType::QString, 0x00095103,
      35, QMetaType::Int, 0x00095103,

 // enums: name, flags, count, data
       7, 0x0,    2,  194,

 // enum data: key, value
      36, uint(ONVIF::NetworkInterfaces::Full),
      37, uint(ONVIF::NetworkInterfaces::Half),

       0        // eod
};

void ONVIF::NetworkInterfaces::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkInterfaces *_t = static_cast<NetworkInterfaces *>(_o);
        switch (_id) {
        case 0: _t->setNetworkInfacesEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setAutoNegotiation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setDuplex((*reinterpret_cast< Duplex(*)>(_a[1]))); break;
        case 4: _t->setMtu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setIpv4Enabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setIpv4ManualAddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setIpv4ManualPrefixLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setNetworkInfacesName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->setHwAaddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setIpv4LinkLocalAddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setIpvLinkLocalPrefixLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setIpv4FromDHCPAddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->setIpv4FromDHCPPrefixLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->setIpv4DHCP((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::NetworkInterfaces::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__NetworkInterfaces.data,
      qt_meta_data_ONVIF__NetworkInterfaces,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::NetworkInterfaces::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::NetworkInterfaces::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__NetworkInterfaces.stringdata))
        return static_cast<void*>(const_cast< NetworkInterfaces*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::NetworkInterfaces::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = result(); break;
        case 1: *reinterpret_cast< bool*>(_v) = networkInfacesEnabled(); break;
        case 2: *reinterpret_cast< bool*>(_v) = autoNegotiation(); break;
        case 3: *reinterpret_cast< int*>(_v) = speed(); break;
        case 4: *reinterpret_cast< Duplex*>(_v) = duplex(); break;
        case 5: *reinterpret_cast< int*>(_v) = mtu(); break;
        case 6: *reinterpret_cast< bool*>(_v) = ipv4Enabled(); break;
        case 7: *reinterpret_cast< QString*>(_v) = ipv4ManualAddress(); break;
        case 8: *reinterpret_cast< int*>(_v) = ipv4ManualPrefixLength(); break;
        case 9: *reinterpret_cast< bool*>(_v) = getIpv4DHCP(); break;
        case 10: *reinterpret_cast< QString*>(_v) = networkInfacesName(); break;
        case 11: *reinterpret_cast< QString*>(_v) = hwAaddress(); break;
        case 12: *reinterpret_cast< QString*>(_v) = ipv4LinkLocalAddress(); break;
        case 13: *reinterpret_cast< int*>(_v) = ipvLinkLocalPrefixLength(); break;
        case 14: *reinterpret_cast< QString*>(_v) = ipv4FromDHCPAddress(); break;
        case 15: *reinterpret_cast< int*>(_v) = ipv4FromDHCPPrefixLength(); break;
        default: break;
        }
        _id -= 16;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setResult(*reinterpret_cast< bool*>(_v)); break;
        case 1: setNetworkInfacesEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 2: setAutoNegotiation(*reinterpret_cast< bool*>(_v)); break;
        case 3: setSpeed(*reinterpret_cast< int*>(_v)); break;
        case 4: setDuplex(*reinterpret_cast< Duplex*>(_v)); break;
        case 5: setMtu(*reinterpret_cast< int*>(_v)); break;
        case 6: setIpv4Enabled(*reinterpret_cast< bool*>(_v)); break;
        case 7: setIpv4ManualAddress(*reinterpret_cast< QString*>(_v)); break;
        case 8: setIpv4ManualPrefixLength(*reinterpret_cast< int*>(_v)); break;
        case 9: setIpv4DHCP(*reinterpret_cast< bool*>(_v)); break;
        case 10: setNetworkInfacesName(*reinterpret_cast< QString*>(_v)); break;
        case 11: setHwAaddress(*reinterpret_cast< QString*>(_v)); break;
        case 12: setIpv4LinkLocalAddress(*reinterpret_cast< QString*>(_v)); break;
        case 13: setIpvLinkLocalPrefixLength(*reinterpret_cast< int*>(_v)); break;
        case 14: setIpv4FromDHCPAddress(*reinterpret_cast< QString*>(_v)); break;
        case 15: setIpv4FromDHCPPrefixLength(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
        _id -= 16;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 16;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
