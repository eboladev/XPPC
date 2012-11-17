/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sat 17. Nov 16:36:48 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

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
      12,   11,   11,   11, 0x08,
      34,   11,   11,   11, 0x08,
      53,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
     103,   11,   11,   11, 0x08,
     135,   11,   11,   11, 0x08,
     172,  166,   11,   11, 0x08,
     212,   11,   11,   11, 0x08,
     247,   11,   11,   11, 0x08,
     285,  277,   11,   11, 0x08,
     320,  277,   11,   11, 0x08,
     356,  277,   11,   11, 0x08,
     390,  166,   11,   11, 0x08,
     436,   11,   11,   11, 0x08,
     468,   11,   11,   11, 0x08,
     524,  505,  497,   11, 0x08,
     549,   11,   11,   11, 0x08,
     562,   11,   11,   11, 0x08,
     604,   11,   11,   11, 0x08,
     637,   11,   11,   11, 0x08,
     667,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0onAddReceiptClicked()\0"
    "onJobListClicked()\0onSettingsClicked()\0"
    "on_radioButtonReady_pressed()\0"
    "on_radioButtonWorking_pressed()\0"
    "on_radioButtonClosed_pressed()\0index\0"
    "on_tableViewTicket_clicked(QModelIndex)\0"
    "on_pushButtonSearchClear_clicked()\0"
    "on_pushButtonSearch_clicked()\0checked\0"
    "on_radioButtonClosed_toggled(bool)\0"
    "on_radioButtonWorking_toggled(bool)\0"
    "on_radioButtonReady_toggled(bool)\0"
    "on_tableViewTicket_doubleClicked(QModelIndex)\0"
    "on_actionDisconnect_triggered()\0"
    "on_actionConnect_triggered()\0QString\0"
    "ticketStatus,limit\0formTicketQuery(int,int)\0"
    "makeUpdate()\0networkFuckedUpTwo(QNetworkConfiguration)\0"
    "on_actionPrintTicket_triggered()\0"
    "on_actionBranches_triggered()\0"
    "on_actionCloseTicket_triggered()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->onAddReceiptClicked(); break;
        case 1: _t->onJobListClicked(); break;
        case 2: _t->onSettingsClicked(); break;
        case 3: _t->on_radioButtonReady_pressed(); break;
        case 4: _t->on_radioButtonWorking_pressed(); break;
        case 5: _t->on_radioButtonClosed_pressed(); break;
        case 6: _t->on_tableViewTicket_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: _t->on_pushButtonSearchClear_clicked(); break;
        case 8: _t->on_pushButtonSearch_clicked(); break;
        case 9: _t->on_radioButtonClosed_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_radioButtonWorking_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_radioButtonReady_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_tableViewTicket_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 13: _t->on_actionDisconnect_triggered(); break;
        case 14: _t->on_actionConnect_triggered(); break;
        case 15: { QString _r = _t->formTicketQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 16: _t->makeUpdate(); break;
        case 17: _t->networkFuckedUpTwo((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 18: _t->on_actionPrintTicket_triggered(); break;
        case 19: _t->on_actionBranches_triggered(); break;
        case 20: _t->on_actionCloseTicket_triggered(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
