/****************************************************************************
** Meta object code from reading C++ file 'mainframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainFrame_t {
    QByteArrayData data[16];
    char stringdata0[347];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainFrame_t qt_meta_stringdata_MainFrame = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MainFrame"
QT_MOC_LITERAL(1, 10, 21), // "on_buttonOpen_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 31), // "on_buttonDeleteContents_clicked"
QT_MOC_LITERAL(4, 65, 26), // "on_buttonSepiaTone_clicked"
QT_MOC_LITERAL(5, 92, 25), // "on_buttonShowList_clicked"
QT_MOC_LITERAL(6, 118, 26), // "on_buttonLuminance_clicked"
QT_MOC_LITERAL(7, 145, 20), // "on_OtsuLabel_clicked"
QT_MOC_LITERAL(8, 166, 25), // "on_GaussianFilter_clicked"
QT_MOC_LITERAL(9, 192, 20), // "on_BoxFilter_clicked"
QT_MOC_LITERAL(10, 213, 23), // "on_MedianFilter_clicked"
QT_MOC_LITERAL(11, 237, 23), // "on_HarrisCorner_clicked"
QT_MOC_LITERAL(12, 261, 16), // "on_Hough_clicked"
QT_MOC_LITERAL(13, 278, 21), // "on_Otical_BTN_clicked"
QT_MOC_LITERAL(14, 300, 19), // "on_NEXT_BTN_clicked"
QT_MOC_LITERAL(15, 320, 26) // "on_Btn_ImgBlending_clicked"

    },
    "MainFrame\0on_buttonOpen_clicked\0\0"
    "on_buttonDeleteContents_clicked\0"
    "on_buttonSepiaTone_clicked\0"
    "on_buttonShowList_clicked\0"
    "on_buttonLuminance_clicked\0"
    "on_OtsuLabel_clicked\0on_GaussianFilter_clicked\0"
    "on_BoxFilter_clicked\0on_MedianFilter_clicked\0"
    "on_HarrisCorner_clicked\0on_Hough_clicked\0"
    "on_Otical_BTN_clicked\0on_NEXT_BTN_clicked\0"
    "on_Btn_ImgBlending_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainFrame *_t = static_cast<MainFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_buttonOpen_clicked(); break;
        case 1: _t->on_buttonDeleteContents_clicked(); break;
        case 2: _t->on_buttonSepiaTone_clicked(); break;
        case 3: _t->on_buttonShowList_clicked(); break;
        case 4: _t->on_buttonLuminance_clicked(); break;
        case 5: _t->on_OtsuLabel_clicked(); break;
        case 6: _t->on_GaussianFilter_clicked(); break;
        case 7: _t->on_BoxFilter_clicked(); break;
        case 8: _t->on_MedianFilter_clicked(); break;
        case 9: _t->on_HarrisCorner_clicked(); break;
        case 10: _t->on_Hough_clicked(); break;
        case 11: _t->on_Otical_BTN_clicked(); break;
        case 12: _t->on_NEXT_BTN_clicked(); break;
        case 13: _t->on_Btn_ImgBlending_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MainFrame::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MainFrame.data,
      qt_meta_data_MainFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrame.stringdata0))
        return static_cast<void*>(const_cast< MainFrame*>(this));
    return QDialog::qt_metacast(_clname);
}

int MainFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
