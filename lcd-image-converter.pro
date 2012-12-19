# -------------------------------------------------
# Project created by QtCreator 2010-06-22T13:10:53
# -------------------------------------------------
OBJECTS_DIR         = .obj
MOC_DIR             = .moc
UI_DIR              = .uic
QT += xml xmlpatterns network
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
    controls/mainwindow.cpp \
    controls/widgetbitmapeditor.cpp \
    controls/editortabimage.cpp \
    controls/editortabfont.cpp \
    controls/dialogsavechanges.cpp \
    classes/convert/bitmaphelper.cpp \
    controls/dialogresize.cpp \
    classes/charactersmodel.cpp \
    controls/dialogfontselect.cpp \
    controls/dialogabout.cpp \
    classes/settings/recentlist.cpp \
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
    classes/convert/bitstream.cpp \
    controls/setup/dialogpreview.cpp \
    classes/preview-models/matrixpreviewmodel.cpp \
    classes/parser.cpp \
    classes/preview-models/matrixitemdelegate.cpp \
    controls/setup/parts/setuptabprepare.cpp \
    controls/setup/parts/setuptabmatrix.cpp \
    controls/setup/parts/setuptabimage.cpp \
    classes/settings/presets/preset.cpp \
    classes/settings/presets/prepareoptions.cpp \
    classes/settings/presets/matrixoptions.cpp \
    classes/settings/presets/imageoptions.cpp \
    classes/settings/presets/templateoptions.cpp \
    controls/setup/parts/setuptabtemplates.cpp \
    controls/setup/dialogoptions.cpp \
    classes/settings/presets/fontoptions.cpp \
    controls/setup/parts/setuptabfont.cpp \
    classes/convert/fonthelper.cpp \
    classes/compression/rlecompressor.cpp \
    classes/fontcharactersmodel.cpp \
    controls/dialogexternaleditor.cpp \
    classes/settings/externaltooloptions.cpp \
    classes/settings/languageoptions.cpp \
    classes/settings/bitmapeditoroptions.cpp \
    controls/dialogfontchanged.cpp \
    classes/data/datacontainer.cpp \
    classes/data/historykeeper.cpp \
    classes/data/historyrecord.cpp
HEADERS += \
    controls/mainwindow.h \
    controls/widgetbitmapeditor.h \
    controls/editortabimage.h \
    controls/editortabfont.h \
    interfaces/idocument.h \
    controls/dialogsavechanges.h \
    classes/convert/bitmaphelper.h \
    controls/dialogresize.h \
    classes/charactersmodel.h \
    controls/dialogfontselect.h \
    controls/dialogabout.h \
    classes/settings/recentlist.h \
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
    classes/convert/bitstream.h \
    controls/setup/dialogpreview.h \
    classes/preview-models/matrixpreviewmodel.h \
    classes/convert/conversion_options.h \
    classes/parser.h \
    classes/preview-models/matrixitemdelegate.h \
    controls/setup/parts/setuptabprepare.h \
    controls/setup/parts/setuptabmatrix.h \
    controls/setup/parts/setuptabimage.h \
    classes/settings/presets/preset.h \
    classes/settings/presets/prepareoptions.h \
    classes/settings/presets/matrixoptions.h \
    classes/settings/presets/imageoptions.h \
    classes/settings/presets/templateoptions.h \
    controls/setup/parts/setuptabtemplates.h \
    controls/setup/dialogoptions.h \
    classes/settings/presets/fontoptions.h \
    controls/setup/parts/setuptabfont.h \
    classes/convert/fonthelper.h \
    classes/compression/rlecompressor.h \
    classes/fontcharactersmodel.h \
    controls/dialogexternaleditor.h \
    classes/settings/externaltooloptions.h \
    classes/settings/languageoptions.h \
    classes/settings/bitmapeditoroptions.h \
    controls/dialogfontchanged.h \
    classes/data/datacontainer.h \
    classes/data/historykeeper.h \
    classes/data/historyrecord.h
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
    controls/setup/parts/setuptabprepare.ui \
    controls/setup/parts/setuptabmatrix.ui \
    controls/setup/parts/setuptabimage.ui \
    controls/setup/parts/setuptabtemplates.ui \
    controls/setup/dialogoptions.ui \
    controls/setup/parts/setuptabfont.ui \
    controls/dialogexternaleditor.ui \
    controls/dialogfontchanged.ui
INCLUDEPATH += . \
    ./classes \
    ./classes/compression \
    ./classes/convert \
    ./classes/data \
    ./classes/settings \
    ./classes/settings/presets \
    ./classes/action-handlers \
    ./classes/preview-models \
    ./controls \
    ./controls/setup \
    ./controls/setup/parts \
    ./interfaces

RESOURCES += \
    resources/resources.qrc

win32 {
    RC_FILE = win-res.rc
    OTHER_FILES += win-res.rc
}

TRANSLATIONS = resources/lcd-image-converter-ru.ts

OTHER_FILES += \
    resources/image.tmpl \
    resources/font.tmpl \
    resources/lcd-image-converter-ru.ts \
    win-res.rc \
    README \
    resources/history.xml \
    resources/history.xsl \
    resources/history.css


# generate version info file on each build, because file in other directory
version.target = version-included.txt
version.commands = @sh ./version-gen.sh
version.depends = .git
QMAKE_EXTRA_TARGETS += version
PRE_TARGETDEPS += version-included.txt

# compile translation
translation_ru.target = ./resources/lcd-image-converter-ru.qm
translation_ru.commands = @sh ./translation-compile.sh
translation_ru.depends = .git
QMAKE_EXTRA_TARGETS += translation_ru
PRE_TARGETDEPS += ./resources/lcd-image-converter-ru.qm ./resources/lcd-image-converter-ru.ts
