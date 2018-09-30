QT       += testlib
#QT       -= gui

OBJECTS_DIR         = .obj
MOC_DIR             = .moc
UI_DIR              = .uic

TARGET = maintest

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QT += xml xmlpatterns network svg

# Widgets in Qt 5
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

# QtScript deprecated in Qt 5.5, replaced by QJSEngine
greaterThan(QT_MAJOR_VERSION, 4) {
  greaterThan(QT_MINOR_VERSION, 4) {
    QT += qml
  }
  lessThan(QT_MINOR_VERSION, 5) {
    QT += script
  }
}
lessThan(QT_MAJOR_VERSION, 5) {
  QT += script
}

DEFINES += QT_MAJOR_VERSION="$$QT_MAJOR_VERSION"
DEFINES += QT_MINOR_VERSION="$$QT_MINOR_VERSION"
DEFINES += QT_PATCH_VERSION="$$QT_PATCH_VERSION"

unix:DESTDIR        = ./_linux
win32:DESTDIR       = ./_windows

SOURCES += maintest.cpp \
    parts/testbitmaphelper.cpp \
    parts/testbitstream.cpp \
    parts/testconverterhelper.cpp \
    parts/testfontoptions.cpp \
    parts/testimageoptions.cpp \
    parts/testmatrixoptions.cpp \
    parts/testprepareoptions.cpp \
    parts/testpresets.cpp \
    parts/testreorderingoptions.cpp \
    parts/testrle.cpp \
    parts/testsettings.cpp \
    parts/testtemplateoptions.cpp \
    ../classes/compression/rlecompressor.cpp \
    ../classes/compression/rlesequence.cpp \
    ../classes/parser/convert/bitstream.cpp \
    ../classes/parser/convert/bitmaphelper.cpp \
    ../classes/parser/convert/converterhelper.cpp \
    ../classes/parser/convert/convimagepixels.cpp \
    ../classes/parser/convert/convimagescan.cpp \
    ../classes/settings/appsettings.cpp \
    ../classes/settings/presets/preset.cpp \
    ../classes/settings/presets/prepareoptions.cpp \
    ../classes/settings/presets/matrixoptions.cpp \
    ../classes/settings/presets/imageoptions.cpp \
    ../classes/settings/presets/fontoptions.cpp \
    ../classes/settings/presets/reorderingoptions.cpp \
    ../classes/settings/presets/templateoptions.cpp

HEADERS += \
    parts/testbitmaphelper.h \
    parts/testbitstream.h \
    parts/testconverterhelper.h \
    parts/testfontoptions.h \
    parts/testimageoptions.h \
    parts/testmatrixoptions.h \
    parts/testprepareoptions.h \
    parts/testpresets.h \
    parts/testreorderingoptions.h \
    parts/testrle.h \
    parts/testsettings.h \
    parts/testtemplateoptions.h \
    ../classes/compression/rlecompressor.h \
    ../classes/compression/rlesequence.h \
    ../classes/parser/convert/bitstream.h \
    ../classes/parser/convert/bitmaphelper.h \
    ../classes/parser/convert/conversion_options.h \
    ../classes/parser/convert/converterhelper.h \
    ../classes/parser/convert/convimagepixels.h \
    ../classes/parser/convert/convimagescan.h \
    ../classes/settings/appsettings.h \
    ../classes/settings/presets/preset.h \
    ../classes/settings/presets/prepareoptions.h \
    ../classes/settings/presets/matrixoptions.h \
    ../classes/settings/presets/imageoptions.h \
    ../classes/settings/presets/fontoptions.h \
    ../classes/settings/presets/reorderingoptions.h \
    ../classes/settings/presets/templateoptions.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += . \
    ../ \
    ./parts \
    ../classes \
    ../classes/compression \
    ../classes/parser/convert \
    ../classes/settings \
    ../classes/settings/presets

RESOURCES += \
    samples.qrc

DISTFILES += \
    ../astyle.astylerc
