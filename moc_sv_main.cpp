/****************************************************************************
** Meta object code from reading C++ file 'sv_main.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "sv_main.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainApp_t {
    QByteArrayData data[21];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainApp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainApp_t qt_meta_stringdata_MainApp = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MainApp"
QT_MOC_LITERAL(1, 8, 14), // "onLoginClicked"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "onClear"
QT_MOC_LITERAL(4, 32, 16), // "onSendOTPClicked"
QT_MOC_LITERAL(5, 49, 8), // "onSearch"
QT_MOC_LITERAL(6, 58, 9), // "onRefresh"
QT_MOC_LITERAL(7, 68, 8), // "onUpload"
QT_MOC_LITERAL(8, 77, 15), // "showContextMenu"
QT_MOC_LITERAL(9, 93, 3), // "pos"
QT_MOC_LITERAL(10, 97, 11), // "previewFile"
QT_MOC_LITERAL(11, 109, 6), // "fileId"
QT_MOC_LITERAL(12, 116, 8), // "FileItem"
QT_MOC_LITERAL(13, 125, 4), // "file"
QT_MOC_LITERAL(14, 130, 12), // "downloadFile"
QT_MOC_LITERAL(15, 143, 8), // "fileName"
QT_MOC_LITERAL(16, 152, 10), // "renameFile"
QT_MOC_LITERAL(17, 163, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(18, 180, 4), // "item"
QT_MOC_LITERAL(19, 185, 11), // "currentName"
QT_MOC_LITERAL(20, 197, 10) // "deleteFile"

    },
    "MainApp\0onLoginClicked\0\0onClear\0"
    "onSendOTPClicked\0onSearch\0onRefresh\0"
    "onUpload\0showContextMenu\0pos\0previewFile\0"
    "fileId\0FileItem\0file\0downloadFile\0"
    "fileName\0renameFile\0QListWidgetItem*\0"
    "item\0currentName\0deleteFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainApp[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    1,   75,    2, 0x08 /* Private */,
      10,    2,   78,    2, 0x08 /* Private */,
      14,    2,   83,    2, 0x08 /* Private */,
      16,    3,   88,    2, 0x08 /* Private */,
      20,    2,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    9,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,   15,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QString, QMetaType::QString,   18,   11,   19,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QString,   18,   11,

       0        // eod
};

void MainApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainApp *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onLoginClicked(); break;
        case 1: _t->onClear(); break;
        case 2: _t->onSendOTPClicked(); break;
        case 3: _t->onSearch(); break;
        case 4: _t->onRefresh(); break;
        case 5: _t->onUpload(); break;
        case 6: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->previewFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const FileItem(*)>(_a[2]))); break;
        case 8: _t->downloadFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->renameFile((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->deleteFile((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainApp::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainApp.data,
    qt_meta_data_MainApp,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainApp.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
