/****************************************************************************
** Meta object code from reading C++ file 'TextureDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../header/TextureDockWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextureDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureDockWidget_t {
    QByteArrayData data[17];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureDockWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureDockWidget_t qt_meta_stringdata_TextureDockWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TextureDockWidget"
QT_MOC_LITERAL(1, 18, 20), // "redNuageValueChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 8), // "redValue"
QT_MOC_LITERAL(4, 49, 22), // "greenNuageValueChanged"
QT_MOC_LITERAL(5, 72, 10), // "greenValue"
QT_MOC_LITERAL(6, 83, 21), // "blueNuageValueChanged"
QT_MOC_LITERAL(7, 105, 9), // "blueValue"
QT_MOC_LITERAL(8, 115, 22), // "absorptionValueChanged"
QT_MOC_LITERAL(9, 138, 15), // "absorptionValue"
QT_MOC_LITERAL(10, 154, 26), // "redNuageSpinBoxChangedSlot"
QT_MOC_LITERAL(11, 181, 5), // "value"
QT_MOC_LITERAL(12, 187, 28), // "greenNuageSpinBoxChangedSlot"
QT_MOC_LITERAL(13, 216, 27), // "blueNuageSpinBoxChangedSlot"
QT_MOC_LITERAL(14, 244, 28), // "absorptionSpinBoxChangedSlot"
QT_MOC_LITERAL(15, 273, 27), // "absorptionSliderChangedSlot"
QT_MOC_LITERAL(16, 301, 1) // "i"

    },
    "TextureDockWidget\0redNuageValueChanged\0"
    "\0redValue\0greenNuageValueChanged\0"
    "greenValue\0blueNuageValueChanged\0"
    "blueValue\0absorptionValueChanged\0"
    "absorptionValue\0redNuageSpinBoxChangedSlot\0"
    "value\0greenNuageSpinBoxChangedSlot\0"
    "blueNuageSpinBoxChangedSlot\0"
    "absorptionSpinBoxChangedSlot\0"
    "absorptionSliderChangedSlot\0i"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureDockWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       6,    1,   65,    2, 0x06 /* Public */,
       8,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   71,    2, 0x08 /* Private */,
      12,    1,   74,    2, 0x08 /* Private */,
      13,    1,   77,    2, 0x08 /* Private */,
      14,    1,   80,    2, 0x08 /* Private */,
      15,    1,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Void, QMetaType::Float,    7,
    QMetaType::Void, QMetaType::Float,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Int,   16,

       0        // eod
};

void TextureDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureDockWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->redNuageValueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->greenNuageValueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->blueNuageValueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->absorptionValueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->redNuageSpinBoxChangedSlot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->greenNuageSpinBoxChangedSlot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->blueNuageSpinBoxChangedSlot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->absorptionSpinBoxChangedSlot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->absorptionSliderChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TextureDockWidget::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureDockWidget::redNuageValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TextureDockWidget::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureDockWidget::greenNuageValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TextureDockWidget::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureDockWidget::blueNuageValueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TextureDockWidget::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureDockWidget::absorptionValueChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TextureDockWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QDockWidget::staticMetaObject>(),
    qt_meta_stringdata_TextureDockWidget.data,
    qt_meta_data_TextureDockWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TextureDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureDockWidget.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int TextureDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TextureDockWidget::redNuageValueChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextureDockWidget::greenNuageValueChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TextureDockWidget::blueNuageValueChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TextureDockWidget::absorptionValueChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
