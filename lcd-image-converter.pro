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
    classes/action-handlers/actionedithandlers.cpp \
    classes/action-handlers/actionfilehandlers.cpp \
    classes/action-handlers/actionfonthandlers.cpp \
    classes/action-handlers/actionhandlersbase.cpp \
    classes/action-handlers/actionhelphandlers.cpp \
    classes/action-handlers/actionimagehandlers.cpp \
    classes/action-handlers/actionsetuphandlers.cpp \
    classes/charactersmodel.cpp \
    classes/compression/rlecompressor.cpp \
    classes/convert/bitmaphelper.cpp \
    classes/convert/bitstream.cpp \
    classes/convert/converterhelper.cpp \
    classes/convert/fonthelper.cpp \
    classes/data/datacontainer.cpp \
    classes/data/historykeeper.cpp \
    classes/data/historyrecord.cpp \
    classes/fontcharactersmodel.cpp \
    classes/parser.cpp \
    classes/preview-models/matrixitemdelegate.cpp \
    classes/preview-models/matrixpreviewmodel.cpp \
    classes/preview-models/reorderingitemdelegate.cpp \
    classes/preview-models/reorderingpreviewmodel.cpp \
    classes/revisioninfo.cpp \
    classes/settings/bitmapeditoroptions.cpp \
    classes/settings/externaltooloptions.cpp \
    classes/settings/languageoptions.cpp \
    classes/settings/presets/fontoptions.cpp \
    classes/settings/presets/imageoptions.cpp \
    classes/settings/presets/matrixoptions.cpp \
    classes/settings/presets/prepareoptions.cpp \
    classes/settings/presets/preset.cpp \
    classes/settings/presets/reorderingoptions.cpp \
    classes/settings/presets/templateoptions.cpp \
    classes/settings/recentlist.cpp \
    controls/dialogabout.cpp \
    controls/dialogexternaleditor.cpp \
    controls/dialogresize.cpp \
    controls/dialogsavechanges.cpp \
    controls/editortabimage.cpp \
    controls/fonts/dialogfontchanged.cpp \
    controls/fonts/dialogfontpreview.cpp \
    controls/fonts/dialogfontselect.cpp \
    controls/fonts/editortabfont.cpp \
    controls/mainwindow.cpp \
    controls/revisionlabel.cpp \
    controls/setup/dialogoptions.cpp \
    controls/setup/dialogpreview.cpp \
    controls/setup/parts/setuptabfont.cpp \
    controls/setup/parts/setuptabimage.cpp \
    controls/setup/parts/setuptabmatrix.cpp \
    controls/setup/parts/setuptabprepare.cpp \
    controls/setup/parts/setuptabreordering.cpp \
    controls/setup/parts/setuptabtemplates.cpp \
    controls/starttab.cpp \
    controls/widgetbitmapeditor.cpp \
    classes/unicodeblocksmodel.cpp

HEADERS += \
    classes/action-handlers/actionedithandlers.h \
    classes/action-handlers/actionfilehandlers.h \
    classes/action-handlers/actionfonthandlers.h \
    classes/action-handlers/actionhandlersbase.h \
    classes/action-handlers/actionhelphandlers.h \
    classes/action-handlers/actionimagehandlers.h \
    classes/action-handlers/actionsetuphandlers.h \
    classes/charactersmodel.h \
    classes/compression/rlecompressor.h \
    classes/convert/bitmaphelper.h \
    classes/convert/bitstream.h \
    classes/convert/conversion_options.h \
    classes/convert/converterhelper.h \
    classes/convert/fonthelper.h \
    classes/data/datacontainer.h \
    classes/data/historykeeper.h \
    classes/data/historyrecord.h \
    classes/fontcharactersmodel.h \
    classes/parser.h \
    classes/preview-models/matrixitemdelegate.h \
    classes/preview-models/matrixpreviewmodel.h \
    classes/preview-models/reorderingitemdelegate.h \
    classes/preview-models/reorderingpreviewmodel.h \
    classes/revisioninfo.h \
    classes/settings/bitmapeditoroptions.h \
    classes/settings/externaltooloptions.h \
    classes/settings/languageoptions.h \
    classes/settings/presets/fontoptions.h \
    classes/settings/presets/imageoptions.h \
    classes/settings/presets/matrixoptions.h \
    classes/settings/presets/prepareoptions.h \
    classes/settings/presets/preset.h \
    classes/settings/presets/reorderingoptions.h \
    classes/settings/presets/templateoptions.h \
    classes/settings/recentlist.h \
    controls/dialogabout.h \
    controls/dialogexternaleditor.h \
    controls/dialogresize.h \
    controls/dialogsavechanges.h \
    controls/editortabimage.h \
    controls/fonts/dialogfontchanged.h \
    controls/fonts/dialogfontpreview.h \
    controls/fonts/dialogfontselect.h \
    controls/fonts/editortabfont.h \
    controls/mainwindow.h \
    controls/revisionlabel.h \
    controls/setup/dialogoptions.h \
    controls/setup/dialogpreview.h \
    controls/setup/parts/setuptabfont.h \
    controls/setup/parts/setuptabimage.h \
    controls/setup/parts/setuptabmatrix.h \
    controls/setup/parts/setuptabprepare.h \
    controls/setup/parts/setuptabreordering.h \
    controls/setup/parts/setuptabtemplates.h \
    controls/starttab.h \
    controls/widgetbitmapeditor.h \
    interfaces/idocument.h \
    interfaces/imainwindow.h \
    classes/unicodeblocksmodel.h

FORMS += \
    controls/dialogabout.ui \
    controls/dialogexternaleditor.ui \
    controls/dialogresize.ui \
    controls/dialogsavechanges.ui \
    controls/editortabimage.ui \
    controls/fonts/dialogfontchanged.ui \
    controls/fonts/dialogfontpreview.ui \
    controls/fonts/dialogfontselect.ui \
    controls/fonts/editortabfont.ui \
    controls/mainwindow.ui \
    controls/setup/dialogoptions.ui \
    controls/setup/dialogpreview.ui \
    controls/setup/parts/setuptabfont.ui \
    controls/setup/parts/setuptabimage.ui \
    controls/setup/parts/setuptabmatrix.ui \
    controls/setup/parts/setuptabprepare.ui \
    controls/setup/parts/setuptabreordering.ui \
    controls/setup/parts/setuptabtemplates.ui \
    controls/starttab.ui \
    controls/widgetbitmapeditor.ui

INCLUDEPATH += . \
    ./classes \
    ./classes/action-handlers \
    ./classes/compression \
    ./classes/convert \
    ./classes/data \
    ./classes/preview-models \
    ./classes/settings \
    ./classes/settings/presets \
    ./controls \
    ./controls/fonts \
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
    resources/history.css \
    resources/unicode_blocks.txt


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
