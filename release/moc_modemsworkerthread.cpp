/****************************************************************************
** Meta object code from reading C++ file 'modemsworkerthread.h'
**
** Created: Sun 18. Nov 13:53:10 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../modemsworkerthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modemsworkerthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModemsWorkerThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   20,   19,   19, 0x05,
      93,   77,   19,   19, 0x05,
     116,   77,   19,   19, 0x05,
     145,   77,   19,   19, 0x05,
     199,  179,   19,   19, 0x05,
     259,  229,   19,   19, 0x05,
     315,  296,   19,   19, 0x05,
     362,  345,   19,   19, 0x05,
     412,  393,   19,   19, 0x05,
     452,  442,   19,   19, 0x05,
     472,  442,   19,   19, 0x05,
     515,  496,   19,   19, 0x05,
     537,  496,   19,   19, 0x05,
     581,  559,   19,   19, 0x05,
     621,  604,   19,   19, 0x05,
     676,  656,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     740,  711,  706,   19, 0x0a,
     793,  774,  706,   19, 0x2a,
     819,  442,  706,   19, 0x0a,
     840,  442,  706,   19, 0x0a,
     860,  442,  706,   19, 0x0a,
     879,  442,  706,   19, 0x0a,
     899,  442,  706,   19, 0x0a,
     922,  442,  706,   19, 0x0a,
     979,  953,  706,   19, 0x0a,
    1034, 1008,  706,   19, 0x0a,
    1067,   19,   19,   19, 0x0a,
    1090, 1085,   19,   19, 0x0a,
    1113,   19,   19,   19, 0x08,
    1127,   19,   19,   19, 0x08,
    1147,   19,   19,   19, 0x08,
    1171,   19,   19,   19, 0x08,
    1194,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ModemsWorkerThread[] = {
    "ModemsWorkerThread\0\0modemName,idSMS,delivered\0"
    "smsDelivered(QString,int,bool)\0"
    "modemName,idSMS\0smsSended(QString,int)\0"
    "startSendingSMS(QString,int)\0"
    "errorWhileSendingSMS(QString,int)\0"
    "modemName,telNumber\0incomingRing(QString,QString)\0"
    "modemName,phoneNumber,smsTest\0"
    "incomingSMS(QString,QString,QString)\0"
    "modemName,rssi,bor\0signalLevels(QString,int,int)\0"
    "modemName,isOpen\0portStateChanged(QString,bool)\0"
    "modemName,response\0isModemResponse(QString,bool)\0"
    "modemName\0portIsLost(QString)\0"
    "portIsRestored(QString)\0modemName,errorNum\0"
    "CMEError(QString,int)\0CMSError(QString,int)\0"
    "modemName,lastCommand\0error(QString,QString)\0"
    "modemName,status\0isRegisteredInNetwork(QString,int)\0"
    "modemName,operatorN\0operatorName(QString,QString)\0"
    "bool\0modemName,portName,telNumber\0"
    "addModem(QString,QString,QString)\0"
    "modemName,portName\0addModem(QString,QString)\0"
    "removeModem(QString)\0closeModem(QString)\0"
    "openModem(QString)\0portIsOpen(QString)\0"
    "portIsPresent(QString)\0"
    "isRegisteredInNetwork(QString)\0"
    "smsText,phoneNumber,idSMS\0"
    "sendSMS(QString,QString,int)\0"
    "oldModemName,newModemName\0"
    "changeModemName(QString,QString)\0"
    "removeAllModems()\0msec\0setMsecToHangDown(int)\0"
    "onReadyRead()\0onCheckPortsTimer()\0"
    "onTimeoutWatcherTimer()\0onCheckOutputBuffers()\0"
    "onHangDownTimer()\0"
};

void ModemsWorkerThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModemsWorkerThread *_t = static_cast<ModemsWorkerThread *>(_o);
        switch (_id) {
        case 0: _t->smsDelivered((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->smsSended((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->startSendingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->errorWhileSendingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->incomingRing((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->incomingSMS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: _t->signalLevels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->portStateChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->isModemResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->portIsLost((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->portIsRestored((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->CMEError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->CMSError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->error((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 14: _t->isRegisteredInNetwork((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->operatorName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 16: { bool _r = _t->addModem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->addModem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->removeModem((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { bool _r = _t->closeModem((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: { bool _r = _t->openModem((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 21: { bool _r = _t->portIsOpen((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: { bool _r = _t->portIsPresent((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 23: { bool _r = _t->isRegisteredInNetwork((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = _t->sendSMS((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: { bool _r = _t->changeModemName((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 26: _t->removeAllModems(); break;
        case 27: _t->setMsecToHangDown((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 28: _t->onReadyRead(); break;
        case 29: _t->onCheckPortsTimer(); break;
        case 30: _t->onTimeoutWatcherTimer(); break;
        case 31: _t->onCheckOutputBuffers(); break;
        case 32: _t->onHangDownTimer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModemsWorkerThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModemsWorkerThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ModemsWorkerThread,
      qt_meta_data_ModemsWorkerThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModemsWorkerThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModemsWorkerThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModemsWorkerThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModemsWorkerThread))
        return static_cast<void*>(const_cast< ModemsWorkerThread*>(this));
    return QObject::qt_metacast(_clname);
}

int ModemsWorkerThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void ModemsWorkerThread::smsDelivered(QString _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ModemsWorkerThread::smsSended(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ModemsWorkerThread::startSendingSMS(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ModemsWorkerThread::errorWhileSendingSMS(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ModemsWorkerThread::incomingRing(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ModemsWorkerThread::incomingSMS(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ModemsWorkerThread::signalLevels(QString _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ModemsWorkerThread::portStateChanged(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ModemsWorkerThread::isModemResponse(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ModemsWorkerThread::portIsLost(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ModemsWorkerThread::portIsRestored(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ModemsWorkerThread::CMEError(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ModemsWorkerThread::CMSError(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ModemsWorkerThread::error(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void ModemsWorkerThread::isRegisteredInNetwork(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void ModemsWorkerThread::operatorName(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_END_MOC_NAMESPACE
