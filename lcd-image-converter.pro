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
    controls/editortabfont.cpp \
    controls/dialogsavechanges.cpp \
    classes/bitmaphelper.cpp \
    controls/dialogresize.cpp \
    classes/fontcontainer.cpp \
    classes/charactersmodel.cpp \
    controls/dialogfontselect.cpp \
    controls/dialogabout.cpp \
    classes/recentlist.cpp \
    controls/starttab.cpp \
    classes/action-handlers/actionimagehandlers.cpp \
    classes/action-handlers/actionhandlersbase.cpp \
    classes/action-handlers/actionfonthandlers.cpp \
    classes/action-handlers/actionhelphandlers.cpp \
    classes/action-handlers/actionsetuphandlers.cpp \
    classes/action-handlers/actionfilehandlers.cpp \
    controls/dialogfontpreview.cpp \
    controls/revisionlabel.cpp \
    classes/revisioninfo.cpp \
    classes/convert/converterhelper.cpp \
    classes/convert/conversionmatrixoptions.cpp \
    classes/convert/bitstream.cpp \
    controls/setup/dialogpreview.cpp \
    classes/preview-models/matrixpreviewmodel.cpp \
    classes/convert/conversionmatrix.cpp \
    controls/setup/dialogconvert.cpp \
    controls/setup/dialogsetup.cpp \
    classes/parser.cpp \
    classes/preview-models/matrixitemdelegate.cpp
HEADERS += \
    classes/bitmapcontainer.h \
    controls/mainwindow.h \
    controls/widgetbitmapeditor.h \
    controls/editortabimage.h \
    controls/editortabfont.h \
    interfaces/idocument.h \
    controls/dialogsavechanges.h \
    classes/bitmaphelper.h \
    interfaces/idatacontainer.h \
    controls/dialogresize.h \
    classes/fontcontainer.h \
    classes/charactersmodel.h \
    controls/dialogfontselect.h \
    controls/dialogabout.h \
    classes/recentlist.h \
    controls/starttab.h \
    classes/action-handlers/actionimagehandlers.h \
    interfaces/imainwindow.h \
    classes/action-handlers/actionhandlersbase.h \
    classes/action-handlers/actionfonthandlers.h \
    classes/action-handlers/actionhelphandlers.h \
    classes/action-handlers/actionsetuphandlers.h \
    classes/action-handlers/actionfilehandlers.h \
    controls/dialogfontpreview.h \
    controls/revisionlabel.h \
    classes/revisioninfo.h \
    classes/convert/converterhelper.h \
    classes/convert/conversionmatrixoptions.h \
    classes/convert/bitstream.h \
    controls/setup/dialogpreview.h \
    classes/preview-models/matrixpreviewmodel.h \
    classes/convert/conversion_options.h \
    classes/convert/conversionmatrix.h \
    controls/setup/dialogconvert.h \
    controls/setup/dialogsetup.h \
    classes/parser.h \
    classes/preview-models/matrixitemdelegate.h
FORMS += \
    controls/mainwindow.ui \
    controls/widgetbitmapeditor.ui \
    controls/editortabimage.ui \
    controls/editortabfont.ui \
    controls/dialogsavechanges.ui \
    controls/dialogresize.ui \
    controls/dialogfontselect.ui \
    controls/dialogabout.ui \
    controls/starttab.ui \
    controls/dialogfontpreview.ui \
    controls/setup/dialogpreview.ui \
    controls/setup/dialogconvert.ui \
    controls/setup/dialogsetup.ui
INCLUDEPATH += . \
    ./classes \
    ./classes/convert \
    ./classes/action-handlers \
    ./classes/preview-models \
    ./controls \
    ./controls/setup \
    ./interfaces

RESOURCES += \
    resources/resources.qrc

win32 {
    RC_FILE = iconrc.rc
    OTHER_FILES += iconrc.rc
}

TRANSLATIONS = resources/lcd-image-converter-ru.ts

OTHER_FILES += \
    resources/image.tmpl \
    resources/font.tmpl \
    resources/template_keys.txt \
    resources/lcd-image-converter-ru.ts \
    iconrc.rc


# define how to create version info file
version.target = version-included.txt
#version.commands = git log --pretty=format:\"git-commit-info%n%h%n%ad\" -1 > ./resources/version-included.txt
version.commands = ./version-gen.sh
version.depends = .git

QMAKE_EXTRA_TARGETS += version

PRE_TARGETDEPS += version-included.txt
