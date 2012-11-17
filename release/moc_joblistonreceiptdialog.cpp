/****************************************************************************
** Meta object code from reading C++ file 'joblistonreceiptdialog.h'
**
** Created: Sat 17. Nov 16:36:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../joblistonreceiptdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'joblistonreceiptdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JobListOnReceiptDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   24,   23,   23, 0x08,
      79,   23,   23,   23, 0x08,
     113,   23,   23,   23, 0x08,
     143,   23,   23,   23, 0x08,
     176,   24,   23,   23, 0x08,
     210,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JobListOnReceiptDialog[] = {
    "JobListOnReceiptDialog\0\0index\0"
    "on_comboBoxEmployeeList_currentIndexChanged(int)\0"
    "on_pushButtonClearField_clicked()\0"
    "on_pushButtonAddJob_clicked()\0"
    "on_pushButtonDeleteJob_clicked()\0"
    "on_tableView_clicked(QModelIndex)\0"
    "on_pushButtonCheckReady_clicked()\0"
};

void JobListOnReceiptDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JobListOnReceiptDialog *_t = static_cast<JobListOnReceiptDialog *>(_o);
        switch (_id) {
        case 0: _t->on_comboBoxEmployeeList_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_pushButtonClearField_clicked(); break;
        case 2: _t->on_pushButtonAddJob_clicked(); break;
        case 3: _t->on_pushButtonDeleteJob_clicked(); break;
        case 4: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->on_pushButtonCheckReady_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JobListOnReceiptDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JobListOnReceiptDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_JobListOnReceiptDialog,
      qt_meta_data_JobListOnReceiptDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JobListOnReceiptDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JobListOnReceiptDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JobListOnReceiptDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JobListOnReceiptDialog))
        return static_cast<void*>(const_cast< JobListOnReceiptDialog*>(this));
    if (!strcmp(_clname, "SqlExtension"))
        return static_cast< SqlExtension*>(const_cast< JobListOnReceiptDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int JobListOnReceiptDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
