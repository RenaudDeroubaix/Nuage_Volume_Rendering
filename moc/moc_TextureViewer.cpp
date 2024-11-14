/****************************************************************************
** Meta object code from reading C++ file 'TextureViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../header/TextureViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextureViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureViewer_t {
    QByteArrayData data[12];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureViewer_t qt_meta_stringdata_TextureViewer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TextureViewer"
QT_MOC_LITERAL(1, 14, 14), // "setImageLabels"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "setRedNuage"
QT_MOC_LITERAL(4, 42, 2), // "_r"
QT_MOC_LITERAL(5, 45, 13), // "setGreenNuage"
QT_MOC_LITERAL(6, 59, 2), // "_g"
QT_MOC_LITERAL(7, 62, 12), // "setBlueNuage"
QT_MOC_LITERAL(8, 75, 2), // "_b"
QT_MOC_LITERAL(9, 78, 18), // "setAbsorptionNuage"
QT_MOC_LITERAL(10, 97, 2), // "_q"
QT_MOC_LITERAL(11, 100, 16) // "recompileShaders"

    },
    "TextureViewer\0setImageLabels\0\0setRedNuage\0"
    "_r\0setGreenNuage\0_g\0setBlueNuage\0_b\0"
    "setAbsorptionNuage\0_q\0recompileShaders"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   45,    2, 0x0a /* Public */,
       5,    1,   48,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x0a /* Public */,
       9,    1,   54,    2, 0x0a /* Public */,
      11,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::Float,   10,
    QMetaType::Void,

       0        // eod
};

void TextureViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureViewer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setImageLabels(); break;
        case 1: _t->setRedNuage((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->setGreenNuage((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setBlueNuage((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->setAbsorptionNuage((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->recompileShaders(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TextureViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureViewer::setImageLabels)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TextureViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QGLViewer::staticMetaObject>(),
    qt_meta_stringdata_TextureViewer.data,
    qt_meta_data_TextureViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TextureViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureViewer.stringdata0))
        return static_cast<void*>(this);
    return QGLViewer::qt_metacast(_clname);
}

int TextureViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TextureViewer::setImageLabels()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
