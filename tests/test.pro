QT       += testlib script
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
    ../classes/parser/convert/converterhelper.cpp \
    ../classes/parser/convert/bitstream.cpp \
    ../classes/parser/convert/bitmaphelper.cpp \
    parts/testconverterhelper.cpp \
    parts/testbitstream.cpp \
    parts/testprepareoptions.cpp \
    ../classes/settings/presets/preset.cpp \
    ../classes/settings/presets/prepareoptions.cpp \
    ../classes/settings/presets/matrixoptions.cpp \
    ../classes/settings/presets/imageoptions.cpp \
    parts/testmatrixoptions.cpp \
    parts/testimageoptions.cpp \
    parts/testpresets.cpp \
    parts/testfontoptions.cpp \
    ../classes/settings/presets/templateoptions.cpp \
    ../classes/settings/presets/fontoptions.cpp \
    parts/testtemplateoptions.cpp \
    ../classes/compression/rlecompressor.cpp \
    parts/testrle.cpp \
    ../classes/settings/presets/reorderingoptions.cpp \
    parts/testreorderingoptions.cpp \
    ../classes/parser/convert/convimage.cpp

HEADERS += \
    parts/testbitmaphelper.h \
    ../classes/parser/convert/converterhelper.h \
    ../classes/convert/conversion_options.h \
    ../classes/parser/convert/bitstream.h \
    ../classes/parser/convert/bitmaphelper.h \
    parts/testconverterhelper.h \
    parts/testbitstream.h \
    parts/testprepareoptions.h \
    ../classes/settings/presets/preset.h \
    ../classes/settings/presets/prepareoptions.h \
    ../classes/settings/presets/matrixoptions.h \
    ../classes/settings/presets/imageoptions.h \
    parts/testmatrixoptions.h \
    parts/testimageoptions.h \
    parts/testpresets.h \
    parts/testfontoptions.h \
    ../classes/settings/presets/templateoptions.h \
    ../classes/settings/presets/fontoptions.h \
    parts/testtemplateoptions.h \
    ../classes/compression/rlecompressor.h \
    parts/testrle.h \
    ../classes/settings/presets/reorderingoptions.h \
    parts/testreorderingoptions.h \
    ../classes/parser/convert/convimage.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += . \
    ./parts \
    ../classes \
    ../classes/parser/convert \
    ../classes/settings/presets \
    ../classes/compression

RESOURCES += \
    samples.qrc
