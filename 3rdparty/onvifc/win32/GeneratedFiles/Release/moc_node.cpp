/****************************************************************************
** Meta object code from reading C++ file 'node.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ptz_management/node.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'node.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ONVIF__Node_t {
    QByteArrayData data[71];
    char stringdata[2237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ONVIF__Node_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ONVIF__Node_t qt_meta_stringdata_ONVIF__Node = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ONVIF::Node"
QT_MOC_LITERAL(1, 12, 7), // "setName"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "arg"
QT_MOC_LITERAL(4, 25, 34), // "setAbsolutePanTiltPositionSpa..."
QT_MOC_LITERAL(5, 60, 40), // "setAbsolutePanTiltPositionSpa..."
QT_MOC_LITERAL(6, 101, 40), // "setAbsolutePanTiltPositionSpa..."
QT_MOC_LITERAL(7, 142, 40), // "setAbsolutePanTiltPositionSpa..."
QT_MOC_LITERAL(8, 183, 40), // "setAbsolutePanTiltPositionSpa..."
QT_MOC_LITERAL(9, 224, 31), // "setAbsoluteZoomPositionSpaceUri"
QT_MOC_LITERAL(10, 256, 37), // "setAbsoluteZoomPositionSpaceX..."
QT_MOC_LITERAL(11, 294, 37), // "setAbsoluteZoomPositionSpaceX..."
QT_MOC_LITERAL(12, 332, 37), // "setRelativePanTiltTranslation..."
QT_MOC_LITERAL(13, 370, 43), // "setRelativePanTiltTranslation..."
QT_MOC_LITERAL(14, 414, 43), // "setRelativePanTiltTranslation..."
QT_MOC_LITERAL(15, 458, 34), // "setRelativeZoomTranslationSpa..."
QT_MOC_LITERAL(16, 493, 40), // "setRelativeZoomTranslationSpa..."
QT_MOC_LITERAL(17, 534, 40), // "setRelativeZoomTranslationSpa..."
QT_MOC_LITERAL(18, 575, 31), // "setContinuousPanTiltVelocityUri"
QT_MOC_LITERAL(19, 607, 37), // "setContinuousPanTiltVelocityX..."
QT_MOC_LITERAL(20, 645, 37), // "setContinuousPanTiltVelocityX..."
QT_MOC_LITERAL(21, 683, 37), // "setContinuousPanTiltVelocityY..."
QT_MOC_LITERAL(22, 721, 37), // "setContinuousPanTiltVelocityY..."
QT_MOC_LITERAL(23, 759, 33), // "setContinuousZoomVelocitySpac..."
QT_MOC_LITERAL(24, 793, 39), // "setContinuousZoomVelocitySpac..."
QT_MOC_LITERAL(25, 833, 39), // "setContinuousZoomVelocitySpac..."
QT_MOC_LITERAL(26, 873, 23), // "setPanTiltSpeedSpaceUri"
QT_MOC_LITERAL(27, 897, 29), // "setPanTiltSpeedSpaceXRangeMin"
QT_MOC_LITERAL(28, 927, 29), // "setPanTiltSpeedSpaceXRangeMax"
QT_MOC_LITERAL(29, 957, 20), // "setZoomSpeedSpaceUri"
QT_MOC_LITERAL(30, 978, 26), // "setZoomSpeedSpaceXRangeMin"
QT_MOC_LITERAL(31, 1005, 26), // "setZoomSpeedSpaceXRangeMax"
QT_MOC_LITERAL(32, 1032, 25), // "setMaximumNumberOfPresets"
QT_MOC_LITERAL(33, 1058, 14), // "setHomeSupport"
QT_MOC_LITERAL(34, 1073, 15), // "setPtzNodeToken"
QT_MOC_LITERAL(35, 1089, 43), // "setRelativePanTiltTranslation..."
QT_MOC_LITERAL(36, 1133, 43), // "setRelativePanTiltTranslation..."
QT_MOC_LITERAL(37, 1177, 12), // "ptzNodeToken"
QT_MOC_LITERAL(38, 1190, 4), // "name"
QT_MOC_LITERAL(39, 1195, 31), // "absolutePanTiltPositionSpaceUri"
QT_MOC_LITERAL(40, 1227, 37), // "absolutePanTiltPositionSpaceX..."
QT_MOC_LITERAL(41, 1265, 37), // "absolutePanTiltPositionSpaceX..."
QT_MOC_LITERAL(42, 1303, 37), // "absolutePanTiltPositionSpaceY..."
QT_MOC_LITERAL(43, 1341, 37), // "absolutePanTiltPositionSpaceY..."
QT_MOC_LITERAL(44, 1379, 28), // "absoluteZoomPositionSpaceUri"
QT_MOC_LITERAL(45, 1408, 34), // "absoluteZoomPositionSpaceXRan..."
QT_MOC_LITERAL(46, 1443, 34), // "absoluteZoomPositionSpaceXRan..."
QT_MOC_LITERAL(47, 1478, 34), // "relativePanTiltTranslationSpa..."
QT_MOC_LITERAL(48, 1513, 40), // "relativePanTiltTranslationSpa..."
QT_MOC_LITERAL(49, 1554, 40), // "relativePanTiltTranslationSpa..."
QT_MOC_LITERAL(50, 1595, 40), // "relativePanTiltTranslationSpa..."
QT_MOC_LITERAL(51, 1636, 40), // "relativePanTiltTranslationSpa..."
QT_MOC_LITERAL(52, 1677, 31), // "relativeZoomTranslationSpaceUri"
QT_MOC_LITERAL(53, 1709, 37), // "relativeZoomTranslationSpaceX..."
QT_MOC_LITERAL(54, 1747, 37), // "relativeZoomTranslationSpaceX..."
QT_MOC_LITERAL(55, 1785, 28), // "continuousPanTiltVelocityUri"
QT_MOC_LITERAL(56, 1814, 34), // "continuousPanTiltVelocityXRan..."
QT_MOC_LITERAL(57, 1849, 34), // "continuousPanTiltVelocityXRan..."
QT_MOC_LITERAL(58, 1884, 34), // "continuousPanTiltVelocityYRan..."
QT_MOC_LITERAL(59, 1919, 34), // "continuousPanTiltVelocityYRan..."
QT_MOC_LITERAL(60, 1954, 30), // "continuousZoomVelocitySpaceUri"
QT_MOC_LITERAL(61, 1985, 36), // "continuousZoomVelocitySpaceXR..."
QT_MOC_LITERAL(62, 2022, 36), // "continuousZoomVelocitySpaceXR..."
QT_MOC_LITERAL(63, 2059, 20), // "panTiltSpeedSpaceUri"
QT_MOC_LITERAL(64, 2080, 26), // "panTiltSpeedSpaceXRangeMin"
QT_MOC_LITERAL(65, 2107, 26), // "panTiltSpeedSpaceXRangeMax"
QT_MOC_LITERAL(66, 2134, 17), // "zoomSpeedSpaceUri"
QT_MOC_LITERAL(67, 2152, 23), // "zoomSpeedSpaceXRangeMin"
QT_MOC_LITERAL(68, 2176, 23), // "zoomSpeedSpaceXRangeMax"
QT_MOC_LITERAL(69, 2200, 22), // "maximumNumberOfPresets"
QT_MOC_LITERAL(70, 2223, 13) // "homeSupported"

    },
    "ONVIF::Node\0setName\0\0arg\0"
    "setAbsolutePanTiltPositionSpaceUri\0"
    "setAbsolutePanTiltPositionSpaceXRangeMin\0"
    "setAbsolutePanTiltPositionSpaceXRangeMax\0"
    "setAbsolutePanTiltPositionSpaceYRangeMin\0"
    "setAbsolutePanTiltPositionSpaceYRangeMax\0"
    "setAbsoluteZoomPositionSpaceUri\0"
    "setAbsoluteZoomPositionSpaceXRangeMin\0"
    "setAbsoluteZoomPositionSpaceXRangeMax\0"
    "setRelativePanTiltTranslationSpaceUri\0"
    "setRelativePanTiltTranslationSpaceXRangeMin\0"
    "setRelativePanTiltTranslationSpaceXRangeMax\0"
    "setRelativeZoomTranslationSpaceUri\0"
    "setRelativeZoomTranslationSpaceXRangeMin\0"
    "setRelativeZoomTranslationSpaceXRangeMax\0"
    "setContinuousPanTiltVelocityUri\0"
    "setContinuousPanTiltVelocityXRangeMin\0"
    "setContinuousPanTiltVelocityXRangeMax\0"
    "setContinuousPanTiltVelocityYRangeMin\0"
    "setContinuousPanTiltVelocityYRangeMax\0"
    "setContinuousZoomVelocitySpaceUri\0"
    "setContinuousZoomVelocitySpaceXRangeMin\0"
    "setContinuousZoomVelocitySpaceXRangeMax\0"
    "setPanTiltSpeedSpaceUri\0"
    "setPanTiltSpeedSpaceXRangeMin\0"
    "setPanTiltSpeedSpaceXRangeMax\0"
    "setZoomSpeedSpaceUri\0setZoomSpeedSpaceXRangeMin\0"
    "setZoomSpeedSpaceXRangeMax\0"
    "setMaximumNumberOfPresets\0setHomeSupport\0"
    "setPtzNodeToken\0"
    "setRelativePanTiltTranslationSpaceYRangeMin\0"
    "setRelativePanTiltTranslationSpaceYRangeMax\0"
    "ptzNodeToken\0name\0absolutePanTiltPositionSpaceUri\0"
    "absolutePanTiltPositionSpaceXRangeMin\0"
    "absolutePanTiltPositionSpaceXRangeMax\0"
    "absolutePanTiltPositionSpaceYRangeMin\0"
    "absolutePanTiltPositionSpaceYRangeMax\0"
    "absoluteZoomPositionSpaceUri\0"
    "absoluteZoomPositionSpaceXRangeMin\0"
    "absoluteZoomPositionSpaceXRangeMax\0"
    "relativePanTiltTranslationSpaceUri\0"
    "relativePanTiltTranslationSpaceXRangeMin\0"
    "relativePanTiltTranslationSpaceXRangeMax\0"
    "relativePanTiltTranslationSpaceYRangeMin\0"
    "relativePanTiltTranslationSpaceYRangeMax\0"
    "relativeZoomTranslationSpaceUri\0"
    "relativeZoomTranslationSpaceXRangeMin\0"
    "relativeZoomTranslationSpaceXRangeMax\0"
    "continuousPanTiltVelocityUri\0"
    "continuousPanTiltVelocityXRangeMin\0"
    "continuousPanTiltVelocityXRangeMax\0"
    "continuousPanTiltVelocityYRangeMin\0"
    "continuousPanTiltVelocityYRangeMax\0"
    "continuousZoomVelocitySpaceUri\0"
    "continuousZoomVelocitySpaceXRangeMin\0"
    "continuousZoomVelocitySpaceXRangeMax\0"
    "panTiltSpeedSpaceUri\0panTiltSpeedSpaceXRangeMin\0"
    "panTiltSpeedSpaceXRangeMax\0zoomSpeedSpaceUri\0"
    "zoomSpeedSpaceXRangeMin\0zoomSpeedSpaceXRangeMax\0"
    "maximumNumberOfPresets\0homeSupported"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ONVIF__Node[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
      34,  286, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  184,    2, 0x0a /* Public */,
       4,    1,  187,    2, 0x0a /* Public */,
       5,    1,  190,    2, 0x0a /* Public */,
       6,    1,  193,    2, 0x0a /* Public */,
       7,    1,  196,    2, 0x0a /* Public */,
       8,    1,  199,    2, 0x0a /* Public */,
       9,    1,  202,    2, 0x0a /* Public */,
      10,    1,  205,    2, 0x0a /* Public */,
      11,    1,  208,    2, 0x0a /* Public */,
      12,    1,  211,    2, 0x0a /* Public */,
      13,    1,  214,    2, 0x0a /* Public */,
      14,    1,  217,    2, 0x0a /* Public */,
      15,    1,  220,    2, 0x0a /* Public */,
      16,    1,  223,    2, 0x0a /* Public */,
      17,    1,  226,    2, 0x0a /* Public */,
      18,    1,  229,    2, 0x0a /* Public */,
      19,    1,  232,    2, 0x0a /* Public */,
      20,    1,  235,    2, 0x0a /* Public */,
      21,    1,  238,    2, 0x0a /* Public */,
      22,    1,  241,    2, 0x0a /* Public */,
      23,    1,  244,    2, 0x0a /* Public */,
      24,    1,  247,    2, 0x0a /* Public */,
      25,    1,  250,    2, 0x0a /* Public */,
      26,    1,  253,    2, 0x0a /* Public */,
      27,    1,  256,    2, 0x0a /* Public */,
      28,    1,  259,    2, 0x0a /* Public */,
      29,    1,  262,    2, 0x0a /* Public */,
      30,    1,  265,    2, 0x0a /* Public */,
      31,    1,  268,    2, 0x0a /* Public */,
      32,    1,  271,    2, 0x0a /* Public */,
      33,    1,  274,    2, 0x0a /* Public */,
      34,    1,  277,    2, 0x0a /* Public */,
      35,    1,  280,    2, 0x0a /* Public */,
      36,    1,  283,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,

 // properties: name, type, flags
      37, QMetaType::QString, 0x00095103,
      38, QMetaType::QString, 0x00095103,
      39, QMetaType::QString, 0x00095103,
      40, QMetaType::Float, 0x00095103,
      41, QMetaType::Float, 0x00095103,
      42, QMetaType::Float, 0x00095103,
      43, QMetaType::Float, 0x00095103,
      44, QMetaType::QString, 0x00095103,
      45, QMetaType::Float, 0x00095103,
      46, QMetaType::Float, 0x00095103,
      47, QMetaType::QString, 0x00095103,
      48, QMetaType::Float, 0x00095103,
      49, QMetaType::Float, 0x00095103,
      50, QMetaType::Float, 0x00095103,
      51, QMetaType::Float, 0x00095103,
      52, QMetaType::QString, 0x00095103,
      53, QMetaType::Float, 0x00095103,
      54, QMetaType::Float, 0x00095103,
      55, QMetaType::QString, 0x00095103,
      56, QMetaType::Float, 0x00095103,
      57, QMetaType::Float, 0x00095103,
      58, QMetaType::Float, 0x00095103,
      59, QMetaType::Float, 0x00095103,
      60, QMetaType::QString, 0x00095103,
      61, QMetaType::Float, 0x00095103,
      62, QMetaType::Float, 0x00095103,
      63, QMetaType::QString, 0x00095103,
      64, QMetaType::Float, 0x00095103,
      65, QMetaType::Float, 0x00095103,
      66, QMetaType::QString, 0x00095103,
      67, QMetaType::Float, 0x00095103,
      68, QMetaType::Float, 0x00095103,
      69, QMetaType::Int, 0x00095103,
      70, QMetaType::Bool, 0x00095003,

       0        // eod
};

void ONVIF::Node::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Node *_t = static_cast<Node *>(_o);
        switch (_id) {
        case 0: _t->setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setAbsolutePanTiltPositionSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setAbsolutePanTiltPositionSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setAbsolutePanTiltPositionSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->setAbsolutePanTiltPositionSpaceYRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setAbsolutePanTiltPositionSpaceYRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->setAbsoluteZoomPositionSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setAbsoluteZoomPositionSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->setAbsoluteZoomPositionSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->setRelativePanTiltTranslationSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setRelativePanTiltTranslationSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 11: _t->setRelativePanTiltTranslationSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 12: _t->setRelativeZoomTranslationSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->setRelativeZoomTranslationSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->setRelativeZoomTranslationSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: _t->setContinuousPanTiltVelocityUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->setContinuousPanTiltVelocityXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 17: _t->setContinuousPanTiltVelocityXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 18: _t->setContinuousPanTiltVelocityYRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 19: _t->setContinuousPanTiltVelocityYRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 20: _t->setContinuousZoomVelocitySpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->setContinuousZoomVelocitySpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 22: _t->setContinuousZoomVelocitySpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 23: _t->setPanTiltSpeedSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->setPanTiltSpeedSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 25: _t->setPanTiltSpeedSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 26: _t->setZoomSpeedSpaceUri((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->setZoomSpeedSpaceXRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 28: _t->setZoomSpeedSpaceXRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 29: _t->setMaximumNumberOfPresets((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->setHomeSupport((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->setPtzNodeToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->setRelativePanTiltTranslationSpaceYRangeMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 33: _t->setRelativePanTiltTranslationSpaceYRangeMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ONVIF::Node::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ONVIF__Node.data,
      qt_meta_data_ONVIF__Node,  qt_static_metacall, 0, 0}
};


const QMetaObject *ONVIF::Node::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ONVIF::Node::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ONVIF__Node.stringdata))
        return static_cast<void*>(const_cast< Node*>(this));
    return QObject::qt_metacast(_clname);
}

int ONVIF::Node::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 34;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = ptzNodeToken(); break;
        case 1: *reinterpret_cast< QString*>(_v) = name(); break;
        case 2: *reinterpret_cast< QString*>(_v) = absolutePanTiltPositionSpaceUri(); break;
        case 3: *reinterpret_cast< float*>(_v) = absolutePanTiltPositionSpaceXRangeMin(); break;
        case 4: *reinterpret_cast< float*>(_v) = absolutePanTiltPositionSpaceXRangeMax(); break;
        case 5: *reinterpret_cast< float*>(_v) = absolutePanTiltPositionSpaceYRangeMin(); break;
        case 6: *reinterpret_cast< float*>(_v) = absolutePanTiltPositionSpaceYRangeMax(); break;
        case 7: *reinterpret_cast< QString*>(_v) = absoluteZoomPositionSpaceUri(); break;
        case 8: *reinterpret_cast< float*>(_v) = absoluteZoomPositionSpaceXRangeMin(); break;
        case 9: *reinterpret_cast< float*>(_v) = absoluteZoomPositionSpaceXRangeMax(); break;
        case 10: *reinterpret_cast< QString*>(_v) = relativePanTiltTranslationSpaceUri(); break;
        case 11: *reinterpret_cast< float*>(_v) = relativePanTiltTranslationSpaceXRangeMin(); break;
        case 12: *reinterpret_cast< float*>(_v) = relativePanTiltTranslationSpaceXRangeMax(); break;
        case 13: *reinterpret_cast< float*>(_v) = relativePanTiltTranslationSpaceYRangeMin(); break;
        case 14: *reinterpret_cast< float*>(_v) = relativePanTiltTranslationSpaceYRangeMax(); break;
        case 15: *reinterpret_cast< QString*>(_v) = relativeZoomTranslationSpaceUri(); break;
        case 16: *reinterpret_cast< float*>(_v) = relativeZoomTranslationSpaceXRangeMin(); break;
        case 17: *reinterpret_cast< float*>(_v) = relativeZoomTranslationSpaceXRangeMax(); break;
        case 18: *reinterpret_cast< QString*>(_v) = continuousPanTiltVelocityUri(); break;
        case 19: *reinterpret_cast< float*>(_v) = continuousPanTiltVelocityXRangeMin(); break;
        case 20: *reinterpret_cast< float*>(_v) = continuousPanTiltVelocityXRangeMax(); break;
        case 21: *reinterpret_cast< float*>(_v) = continuousPanTiltVelocityYRangeMin(); break;
        case 22: *reinterpret_cast< float*>(_v) = continuousPanTiltVelocityYRangeMax(); break;
        case 23: *reinterpret_cast< QString*>(_v) = continuousZoomVelocitySpaceUri(); break;
        case 24: *reinterpret_cast< float*>(_v) = continuousZoomVelocitySpaceXRangeMin(); break;
        case 25: *reinterpret_cast< float*>(_v) = continuousZoomVelocitySpaceXRangeMax(); break;
        case 26: *reinterpret_cast< QString*>(_v) = panTiltSpeedSpaceUri(); break;
        case 27: *reinterpret_cast< float*>(_v) = panTiltSpeedSpaceXRangeMin(); break;
        case 28: *reinterpret_cast< float*>(_v) = panTiltSpeedSpaceXRangeMax(); break;
        case 29: *reinterpret_cast< QString*>(_v) = zoomSpeedSpaceUri(); break;
        case 30: *reinterpret_cast< float*>(_v) = zoomSpeedSpaceXRangeMin(); break;
        case 31: *reinterpret_cast< float*>(_v) = zoomSpeedSpaceXRangeMax(); break;
        case 32: *reinterpret_cast< int*>(_v) = maximumNumberOfPresets(); break;
        case 33: *reinterpret_cast< bool*>(_v) = homeSupport(); break;
        default: break;
        }
        _id -= 34;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPtzNodeToken(*reinterpret_cast< QString*>(_v)); break;
        case 1: setName(*reinterpret_cast< QString*>(_v)); break;
        case 2: setAbsolutePanTiltPositionSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 3: setAbsolutePanTiltPositionSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 4: setAbsolutePanTiltPositionSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 5: setAbsolutePanTiltPositionSpaceYRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 6: setAbsolutePanTiltPositionSpaceYRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 7: setAbsoluteZoomPositionSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 8: setAbsoluteZoomPositionSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 9: setAbsoluteZoomPositionSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 10: setRelativePanTiltTranslationSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 11: setRelativePanTiltTranslationSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 12: setRelativePanTiltTranslationSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 13: setRelativePanTiltTranslationSpaceYRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 14: setRelativePanTiltTranslationSpaceYRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 15: setRelativeZoomTranslationSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 16: setRelativeZoomTranslationSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 17: setRelativeZoomTranslationSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 18: setContinuousPanTiltVelocityUri(*reinterpret_cast< QString*>(_v)); break;
        case 19: setContinuousPanTiltVelocityXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 20: setContinuousPanTiltVelocityXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 21: setContinuousPanTiltVelocityYRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 22: setContinuousPanTiltVelocityYRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 23: setContinuousZoomVelocitySpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 24: setContinuousZoomVelocitySpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 25: setContinuousZoomVelocitySpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 26: setPanTiltSpeedSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 27: setPanTiltSpeedSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 28: setPanTiltSpeedSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 29: setZoomSpeedSpaceUri(*reinterpret_cast< QString*>(_v)); break;
        case 30: setZoomSpeedSpaceXRangeMin(*reinterpret_cast< float*>(_v)); break;
        case 31: setZoomSpeedSpaceXRangeMax(*reinterpret_cast< float*>(_v)); break;
        case 32: setMaximumNumberOfPresets(*reinterpret_cast< int*>(_v)); break;
        case 33: setHomeSupport(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 34;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 34;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 34)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 34;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
