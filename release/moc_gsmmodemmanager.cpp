/****************************************************************************
** Meta object code from reading C++ file 'gsmmodemmanager.h'
**
** Created: Sat 17. Nov 16:36:55 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gsmmodemmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gsmmodemmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ItemModelForGSMModemManagement[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   32,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   74,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ItemModelForGSMModemManagement[] = {
    "ItemModelForGSMModemManagement\0\0"
    "modemName\0sameModemNameIsPresent(QString)\0"
    "item\0onItemChanged(QStandardItem*)\0"
};

void ItemModelForGSMModemManagement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ItemModelForGSMModemManagement *_t = static_cast<ItemModelForGSMModemManagement *>(_o);
        switch (_id) {
        case 0: _t->sameModemNameIsPresent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onItemChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ItemModelForGSMModemManagement::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ItemModelForGSMModemManagement::staticMetaObject = {
    { &QStandardItemModel::staticMetaObject, qt_meta_stringdata_ItemModelForGSMModemManagement,
      qt_meta_data_ItemModelForGSMModemManagement, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ItemModelForGSMModemManagement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ItemModelForGSMModemManagement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ItemModelForGSMModemManagement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ItemModelForGSMModemManagement))
        return static_cast<void*>(const_cast< ItemModelForGSMModemManagement*>(this));
    if (!strcmp(_clname, "SqlExtension"))
        return static_cast< SqlExtension*>(const_cast< ItemModelForGSMModemManagement*>(this));
    return QStandardItemModel::qt_metacast(_clname);
}

int ItemModelForGSMModemManagement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStandardItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ItemModelForGSMModemManagement::sameModemNameIsPresent(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_GsmModemManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      37,   16,   16,   16, 0x08,
      64,   60,   16,   16, 0x08,
     107,  101,   16,   16, 0x08,
     144,  134,   16,   16, 0x08,
     204,  178,   16,   16, 0x08,
     253,  237,   16,   16, 0x08,
     278,  237,   16,   16, 0x08,
     309,  237,   16,   16, 0x08,
     365,  345,   16,   16, 0x08,
     427,  397,   16,   16, 0x08,
     485,  466,   16,   16, 0x08,
     534,  517,   16,   16, 0x08,
     586,  567,   16,   16, 0x08,
     618,  134,   16,   16, 0x08,
     640,  134,   16,   16, 0x08,
     685,  666,   16,   16, 0x08,
     709,  666,   16,   16, 0x08,
     755,  733,   16,   16, 0x08,
     797,  780,   16,   16, 0x08,
     855,  835,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GsmModemManager[] = {
    "GsmModemManager\0\0onAddButtonPushed()\0"
    "onRemoveButtonPushed()\0pos\0"
    "onCustomContextMenuRequested(QPoint)\0"
    "index\0onDeleteModem(QModelIndex)\0"
    "modemName\0onSameModemNameIsPresent(QString)\0"
    "modemName,idSMS,delivered\0"
    "onSmsDelivered(QString,int,bool)\0"
    "modemName,idSMS\0onSmsSended(QString,int)\0"
    "onStartSendingSMS(QString,int)\0"
    "onErrorWhileSendingSMS(QString,int)\0"
    "modemName,telNumber\0onIncomingRing(QString,QString)\0"
    "modemName,phoneNumber,smsTest\0"
    "onIncomingSMS(QString,QString,QString)\0"
    "modemName,rssi,bor\0onSignalLevels(QString,int,int)\0"
    "modemName,isOpen\0onPortStateChanged(QString,bool)\0"
    "modemName,response\0onIsModemResponse(QString,bool)\0"
    "onPortIsLost(QString)\0onPortIsRestored(QString)\0"
    "modemName,errorNum\0onCMEError(QString,int)\0"
    "onCMSError(QString,int)\0modemName,lastCommand\0"
    "onError(QString,QString)\0modemName,status\0"
    "onIsRegisteringInNetwork(QString,int)\0"
    "modemName,operatorN\0onOperatorName(QString,QString)\0"
};

void GsmModemManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GsmModemManager *_t = static_cast<GsmModemManager *>(_o);
        switch (_id) {
        case 0: _t->onAddButtonPushed(); break;
        case 1: _t->onRemoveButtonPushed(); break;
        case 2: _t->onCustomContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->onDeleteModem((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->onSameModemNameIsPresent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onSmsDelivered((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->onSmsSended((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onStartSendingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->onErrorWhileSendingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->onIncomingRing((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->onIncomingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: _t->onSignalLevels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: _t->onPortStateChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 13: _t->onIsModemResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 14: _t->onPortIsLost((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->onPortIsRestored((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->onCMEError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->onCMSError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->onError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 19: _t->onIsRegisteringInNetwork((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->onOperatorName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GsmModemManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GsmModemManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GsmModemManager,
      qt_meta_data_GsmModemManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GsmModemManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GsmModemManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GsmModemManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GsmModemManager))
        return static_cast<void*>(const_cast< GsmModemManager*>(this));
    if (!strcmp(_clname, "SqlExtension"))
        return static_cast< SqlExtension*>(const_cast< GsmModemManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int GsmModemManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
