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
    controls/widgetconvoptionsmono.cpp \
    classes/bytelistitemdelegate.cpp \
    controls/widgetconvoptionsgray.cpp \
    controls/widgetconvoptionscolor.cpp \
    controls/dialogconvert.cpp \
    classes/converter.cpp \
    classes/convertermono.cpp \
    classes/convertergrayscale.cpp \
    classes/convertercolor.cpp \
    classes/bitmapdata.cpp \
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
    controls/dialogsetup.cpp \
    classes/convert/converterhelper.cpp \
    classes/convert/conversionmatrixoptions.cpp \
    classes/convert/bitstream.cpp \
    controls/setup/dialogpreview.cpp \
    controls/setup/dialogconvert2.cpp \
    classes/preview-models/sourcepreviewmodel.cpp \
    classes/preview-models/operationspreviewmodel.cpp
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
    controls/widgetconvoptionsmono.h \
    classes/bytelistitemdelegate.h \
    controls/widgetconvoptionsgray.h \
    controls/widgetconvoptionscolor.h \
    controls/dialogconvert.h \
    classes/converter.h \
    classes/convertermono.h \
    classes/convertergrayscale.h \
    classes/convertercolor.h \
    interfaces/iconverter.h \
    classes/bitmapdata.h \
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
    controls/dialogsetup.h \
    classes/convert/converterhelper.h \
    classes/convert/conversionmatrixoptions.h \
    classes/convert/bitstream.h \
    controls/setup/dialogpreview.h \
    controls/setup/dialogconvert2.h \
    classes/preview-models/sourcepreviewmodel.h \
    classes/preview-models/operationspreviewmodel.h
FORMS += \
    controls/mainwindow.ui \
    controls/widgetbitmapeditor.ui \
    controls/editortabimage.ui \
    controls/editortabfont.ui \
    controls/dialogsavechanges.ui \
    controls/dialogresize.ui \
    controls/widgetconvoptionsmono.ui \
    controls/widgetconvoptionsgray.ui \
    controls/widgetconvoptionscolor.ui \
    controls/dialogconvert.ui \
    controls/dialogfontselect.ui \
    controls/dialogabout.ui \
    controls/starttab.ui \
    controls/dialogfontpreview.ui \
    controls/dialogsetup.ui \
    controls/setup/dialogpresets.ui \
    controls/setup/dialogconvert2.ui
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
