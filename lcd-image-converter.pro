# -------------------------------------------------
# Project created by QtCreator 2010-06-22T13:10:53
# -------------------------------------------------
OBJECTS_DIR         = .obj
MOC_DIR             = .moc
UI_DIR              = .uic
QT += xml xmlpatterns
TARGET = lcd-image-converter
TEMPLATE = app

unix:DESTDIR        = ./_linux
win32:DESTDIR       = ./_windows

CONFIG(debug, debug|release) {
    DESTDIR         = $$DESTDIR/debug
    DEFINES        += DEBUG_VERSION
    QMAKE_LIBDIR   += $$DESTDIR
} else {
    DESTDIR         = $$DESTDIR/release
    QMAKE_LIBDIR   += $$DESTDIR
}

SOURCES += main.cpp \
    classes/bitmapcontainer.cpp \
    controls/mainwindow.cpp \
    controls/widgetbitmapeditor.cpp \
    controls/editortabimage.cpp \
    controls/editortabfont.cpp
HEADERS += \
    classes/bitmapcontainer.h \
    interfaces/iedit.h \
    controls/mainwindow.h \
    controls/widgetbitmapeditor.h \
    controls/editortabimage.h \
    controls/editortabfont.h
FORMS += \
    controls/mainwindow.ui \
    controls/widgetbitmapeditor.ui \
    controls/editortabimage.ui \
    controls/editortabfont.ui
INCLUDEPATH += . \
    ./classes \
    ./controls \
    ./interfaces

RESOURCES += \
    resources/resources.qrc
