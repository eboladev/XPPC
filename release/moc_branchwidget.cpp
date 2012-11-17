/****************************************************************************
** Meta object code from reading C++ file 'branchwidget.h'
**
** Created: Sat 17. Nov 16:36:52 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../branchwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'branchwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BranchWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      40,   13,   13,   13, 0x08,
      76,   13,   13,   13, 0x08,
      97,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BranchWidget[] = {
    "BranchWidget\0\0onCurrentBrancheChanged()\0"
    "onBranchViewChanged(QStandardItem*)\0"
    "onAddBranchClicked()\0onRemoveBranchClicked()\0"
};

void BranchWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BranchWidget *_t = static_cast<BranchWidget *>(_o);
        switch (_id) {
        case 0: _t->onCurrentBrancheChanged(); break;
        case 1: _t->onBranchViewChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        case 2: _t->onAddBranchClicked(); break;
        case 3: _t->onRemoveBranchClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BranchWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BranchWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BranchWidget,
      qt_meta_data_BranchWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BranchWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BranchWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BranchWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BranchWidget))
        return static_cast<void*>(const_cast< BranchWidget*>(this));
    if (!strcmp(_clname, "SqlExtension"))
        return static_cast< SqlExtension*>(const_cast< BranchWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int BranchWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
