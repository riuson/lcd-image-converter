QT       += testlib
#QT       -= gui

OBJECTS_DIR         = .obj
MOC_DIR             = .moc
UI_DIR              = .uic

TARGET = maintest

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

unix:DESTDIR        = ./_linux
win32:DESTDIR       = ./_windows

SOURCES += maintest.cpp \
    parts/testbitmaphelper.cpp \
    ../classes/convert/converterhelper.cpp \
    ../classes/convert/conversionmatrixoptions.cpp \
    ../classes/convert/conversionmatrix.cpp \
    ../classes/convert/bitstream.cpp \
    ../classes/convert/bitmaphelper.cpp \
    testconversionmatrixoptions.cpp

HEADERS += \
    parts/testbitmaphelper.h \
    ../classes/convert/converterhelper.h \
    ../classes/convert/conversion_options.h \
    ../classes/convert/conversionmatrixoptions.h \
    ../classes/convert/conversionmatrix.h \
    ../classes/convert/bitstream.h \
    ../classes/convert/bitmaphelper.h \
    testconversionmatrixoptions.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += . \
    ./parts \
    ../classes \
    ../classes/convert

RESOURCES += \
    samples.qrc
