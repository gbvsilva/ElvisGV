/****************************************************************************
** Meta object code from reading C++ file 'GLWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GLWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GLWidget_t {
    QByteArrayData data[27];
    char stringdata[311];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GLWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GLWidget_t qt_meta_stringdata_GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 12),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 10),
QT_MOC_LITERAL(4, 34, 10),
QT_MOC_LITERAL(5, 45, 13),
QT_MOC_LITERAL(6, 59, 12),
QT_MOC_LITERAL(7, 72, 10),
QT_MOC_LITERAL(8, 83, 13),
QT_MOC_LITERAL(9, 97, 11),
QT_MOC_LITERAL(10, 109, 8),
QT_MOC_LITERAL(11, 118, 10),
QT_MOC_LITERAL(12, 129, 15),
QT_MOC_LITERAL(13, 145, 13),
QT_MOC_LITERAL(14, 159, 13),
QT_MOC_LITERAL(15, 173, 11),
QT_MOC_LITERAL(16, 185, 15),
QT_MOC_LITERAL(17, 201, 12),
QT_MOC_LITERAL(18, 214, 12),
QT_MOC_LITERAL(19, 227, 12),
QT_MOC_LITERAL(20, 240, 8),
QT_MOC_LITERAL(21, 249, 10),
QT_MOC_LITERAL(22, 260, 11),
QT_MOC_LITERAL(23, 272, 7),
QT_MOC_LITERAL(24, 280, 8),
QT_MOC_LITERAL(25, 289, 8),
QT_MOC_LITERAL(26, 298, 11)
    },
    "GLWidget\0mouseClicked\0\0mouseMoved\0"
    "mouseClick\0mouseMovement\0drawPolyline\0"
    "drawCircle\0drawRectangle\0drawEllipse\0"
    "editCopy\0editDelete\0editTranslation\0"
    "editSelection\0editVertexSel\0editPolySel\0"
    "editCreateGroup\0editDelGroup\0editDecLayer\0"
    "editIncLayer\0viewGrid\0viewZoomIn\0"
    "viewZoomOut\0viewPan\0fileLoad\0fileSave\0"
    "fileSaveSVG\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  139,    2, 0x06,
       3,    0,  140,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,  141,    2, 0x0a,
       5,    0,  142,    2, 0x0a,
       6,    0,  143,    2, 0x0a,
       7,    0,  144,    2, 0x0a,
       8,    0,  145,    2, 0x0a,
       9,    0,  146,    2, 0x0a,
      10,    0,  147,    2, 0x0a,
      11,    0,  148,    2, 0x0a,
      12,    0,  149,    2, 0x0a,
      13,    0,  150,    2, 0x0a,
      14,    0,  151,    2, 0x0a,
      15,    0,  152,    2, 0x0a,
      16,    0,  153,    2, 0x0a,
      17,    0,  154,    2, 0x0a,
      18,    0,  155,    2, 0x0a,
      19,    0,  156,    2, 0x0a,
      20,    0,  157,    2, 0x0a,
      21,    0,  158,    2, 0x0a,
      22,    0,  159,    2, 0x0a,
      23,    0,  160,    2, 0x0a,
      24,    0,  161,    2, 0x0a,
      25,    0,  162,    2, 0x0a,
      26,    0,  163,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

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

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWidget *_t = static_cast<GLWidget *>(_o);
        switch (_id) {
        case 0: _t->mouseClicked(); break;
        case 1: _t->mouseMoved(); break;
        case 2: _t->mouseClick(); break;
        case 3: _t->mouseMovement(); break;
        case 4: _t->drawPolyline(); break;
        case 5: _t->drawCircle(); break;
        case 6: _t->drawRectangle(); break;
        case 7: _t->drawEllipse(); break;
        case 8: _t->editCopy(); break;
        case 9: _t->editDelete(); break;
        case 10: _t->editTranslation(); break;
        case 11: _t->editSelection(); break;
        case 12: _t->editVertexSel(); break;
        case 13: _t->editPolySel(); break;
        case 14: _t->editCreateGroup(); break;
        case 15: _t->editDelGroup(); break;
        case 16: _t->editDecLayer(); break;
        case 17: _t->editIncLayer(); break;
        case 18: _t->viewGrid(); break;
        case 19: _t->viewZoomIn(); break;
        case 20: _t->viewZoomOut(); break;
        case 21: _t->viewPan(); break;
        case 22: _t->fileLoad(); break;
        case 23: _t->fileSave(); break;
        case 24: _t->fileSaveSVG(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::mouseClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (GLWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::mouseMoved)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLWidget.data,
      qt_meta_data_GLWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget.stringdata))
        return static_cast<void*>(const_cast< GLWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::mouseClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GLWidget::mouseMoved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
