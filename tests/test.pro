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
    parts/testconversionmatrixoptions.cpp \
    parts/testconversionmatrix.cpp \
    parts/testconverterhelper.cpp \
    parts/testbitstream.cpp \
    parts/testprepareoptions.cpp \
    ../classes/presets/preset.cpp \
    ../classes/presets/prepareoptions.cpp \
    ../classes/presets/matrixoptions.cpp \
    ../classes/presets/imageoptions.cpp \
    parts/testmatrixoptions.cpp \
    parts/testimageoptions.cpp \
    parts/testpresets.cpp

HEADERS += \
    parts/testbitmaphelper.h \
    ../classes/convert/converterhelper.h \
    ../classes/convert/conversion_options.h \
    ../classes/convert/conversionmatrixoptions.h \
    ../classes/convert/conversionmatrix.h \
    ../classes/convert/bitstream.h \
    ../classes/convert/bitmaphelper.h \
    parts/testconversionmatrixoptions.h \
    parts/testconversionmatrix.h \
    parts/testconverterhelper.h \
    parts/testbitstream.h \
    parts/testprepareoptions.h \
    ../classes/presets/preset.h \
    ../classes/presets/prepareoptions.h \
    ../classes/presets/matrixoptions.h \
    ../classes/presets/imageoptions.h \
    parts/testmatrixoptions.h \
    parts/testimageoptions.h \
    parts/testpresets.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += . \
    ./parts \
    ../classes \
    ../classes/convert \
    ../classes/presets

RESOURCES += \
    samples.qrc
