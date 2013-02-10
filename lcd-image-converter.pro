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
    classes/compression/rlecompressor.cpp \
    classes/data/datacontainer.cpp \
    classes/data/historykeeper.cpp \
    classes/data/historyrecord.cpp \
    classes/data/imagesmodel.cpp \
    classes/data/revisioninfo.cpp \
    classes/parser/convert/bitmaphelper.cpp \
    classes/parser/convert/bitstream.cpp \
    classes/parser/convert/converterhelper.cpp \
    classes/parser/convert/fonthelper.cpp \
    classes/parser/parser.cpp \
    classes/parser/tags.cpp \
    classes/settings/bitmapeditoroptions.cpp \
    classes/settings/externaltooloptions.cpp \
    classes/settings/fonteditoroptions.cpp \
    classes/settings/languageoptions.cpp \
    classes/settings/presets/fontoptions.cpp \
    classes/settings/presets/imageoptions.cpp \
    classes/settings/presets/matrixoptions.cpp \
    classes/settings/presets/prepareoptions.cpp \
    classes/settings/presets/preset.cpp \
    classes/settings/presets/reorderingoptions.cpp \
    classes/settings/presets/templateoptions.cpp \
    classes/settings/recentlist.cpp \
    classes/settings/resizesettings.cpp \
    classes/status/statusdata.cpp \
    classes/status/statusmanager.cpp \
    controls/about/dialogabout.cpp \
    controls/fonts/font-changed/dialogfontchanged.cpp \
    controls/fonts/font-editor/editortabfont.cpp \
    controls/fonts/font-new/charactersmodel.cpp \
    controls/fonts/font-new/dialogfontselect.cpp \
    controls/fonts/font-new/unicodeblocksmodel.cpp \
    controls/fonts/font-new/unicodeblocksfiltermodel.cpp \
    controls/fonts/font-preview/dialogfontpreview.cpp \
    controls/images/editortabimage.cpp \
    controls/images/widgetbitmapeditor.cpp \
    controls/main/mainwindow.cpp \
    controls/main/revisionlabel.cpp \
    controls/resize/dialogcanvasresize.cpp \
    controls/resize/imagesfilterproxy.cpp \
    controls/save-changes/dialogsavechanges.cpp \
    controls/setup/dialogexternaleditor.cpp \
    controls/setup/dialogoptions.cpp \
    controls/setup/dialogpreview.cpp \
    controls/setup/parts/font/setuptabfont.cpp \
    controls/setup/parts/image/setuptabimage.cpp \
    controls/setup/parts/matrix/matrixitemdelegate.cpp \
    controls/setup/parts/matrix/matrixpreviewmodel.cpp \
    controls/setup/parts/matrix/setuptabmatrix.cpp \
    controls/setup/parts/prepare/setuptabprepare.cpp \
    controls/setup/parts/reordering/reorderingitemdelegate.cpp \
    controls/setup/parts/reordering/reorderingpreviewmodel.cpp \
    controls/setup/parts/reordering/setuptabreordering.cpp \
    controls/setup/parts/templates/setuptabtemplates.cpp \
    controls/start/starttab.cpp \
    controls/updates/dialogupdates.cpp

HEADERS += \
    classes/action-handlers/actionedithandlers.h \
    classes/action-handlers/actionfilehandlers.h \
    classes/action-handlers/actionfonthandlers.h \
    classes/action-handlers/actionhandlersbase.h \
    classes/action-handlers/actionhelphandlers.h \
    classes/action-handlers/actionimagehandlers.h \
    classes/action-handlers/actionsetuphandlers.h \
    classes/compression/rlecompressor.h \
    classes/data/datacontainer.h \
    classes/data/historykeeper.h \
    classes/data/historyrecord.h \
    classes/data/imagesmodel.h \
    classes/data/revisioninfo.h \
    classes/parser/convert/bitmaphelper.h \
    classes/parser/convert/bitstream.h \
    classes/parser/convert/conversion_options.h \
    classes/parser/convert/converterhelper.h \
    classes/parser/convert/fonthelper.h \
    classes/parser/parser.h \
    classes/parser/tags.h \
    classes/settings/bitmapeditoroptions.h \
    classes/settings/externaltooloptions.h \
    classes/settings/fonteditoroptions.h \
    classes/settings/languageoptions.h \
    classes/settings/presets/fontoptions.h \
    classes/settings/presets/imageoptions.h \
    classes/settings/presets/matrixoptions.h \
    classes/settings/presets/prepareoptions.h \
    classes/settings/presets/preset.h \
    classes/settings/presets/reorderingoptions.h \
    classes/settings/presets/templateoptions.h \
    classes/settings/recentlist.h \
    classes/settings/resizesettings.h \
    classes/status/statusdata.h \
    classes/status/statusmanager.h \
    controls/about/dialogabout.h \
    controls/fonts/font-changed/dialogfontchanged.h \
    controls/fonts/font-editor/editortabfont.h \
    controls/fonts/font-new/charactersmodel.h \
    controls/fonts/font-new/dialogfontselect.h \
    controls/fonts/font-new/unicodeblocksmodel.h \
    controls/fonts/font-new/unicodeblocksfiltermodel.h \
    controls/fonts/font-preview/dialogfontpreview.h \
    controls/images/editortabimage.h \
    controls/images/widgetbitmapeditor.h \
    controls/main/mainwindow.h \
    controls/main/revisionlabel.h \
    controls/resize/dialogcanvasresize.h \
    controls/resize/imagesfilterproxy.h \
    controls/save-changes/dialogsavechanges.h \
    controls/setup/dialogexternaleditor.h \
    controls/setup/dialogoptions.h \
    controls/setup/dialogpreview.h \
    controls/setup/parts/font/setuptabfont.h \
    controls/setup/parts/image/setuptabimage.h \
    controls/setup/parts/matrix/matrixitemdelegate.h \
    controls/setup/parts/matrix/matrixpreviewmodel.h \
    controls/setup/parts/matrix/setuptabmatrix.h \
    controls/setup/parts/prepare/setuptabprepare.h \
    controls/setup/parts/reordering/reorderingitemdelegate.h \
    controls/setup/parts/reordering/reorderingpreviewmodel.h \
    controls/setup/parts/reordering/setuptabreordering.h \
    controls/setup/parts/templates/setuptabtemplates.h \
    controls/start/starttab.h \
    controls/updates/dialogupdates.h \
    interfaces/idocument.h \
    interfaces/imainwindow.h

FORMS += \
    controls/about/dialogabout.ui \
    controls/fonts/font-changed/dialogfontchanged.ui \
    controls/fonts/font-editor/editortabfont.ui \
    controls/fonts/font-new/dialogfontselect.ui \
    controls/fonts/font-preview/dialogfontpreview.ui \
    controls/images/editortabimage.ui \
    controls/images/widgetbitmapeditor.ui \
    controls/main/mainwindow.ui \
    controls/resize/dialogcanvasresize.ui \
    controls/save-changes/dialogsavechanges.ui \
    controls/setup/dialogexternaleditor.ui \
    controls/setup/dialogoptions.ui \
    controls/setup/dialogpreview.ui \
    controls/setup/parts/font/setuptabfont.ui \
    controls/setup/parts/image/setuptabimage.ui \
    controls/setup/parts/matrix/setuptabmatrix.ui \
    controls/setup/parts/prepare/setuptabprepare.ui \
    controls/setup/parts/reordering/setuptabreordering.ui \
    controls/setup/parts/templates/setuptabtemplates.ui \
    controls/start/starttab.ui \
    controls/updates/dialogupdates.ui

INCLUDEPATH += . \
    ./classes \
    ./classes/action-handlers \
    ./classes/compression \
    ./classes/data \
    ./classes/status \
    ./classes/parser \
    ./classes/parser/convert \
    ./classes/preview-models \
    ./classes/settings \
    ./classes/settings/presets \
    ./controls \
    ./controls/about \
    ./controls/fonts/font-changed \
    ./controls/fonts/font-editor \
    ./controls/fonts/font-new \
    ./controls/fonts/font-preview \
    ./controls/images \
    ./controls/main \
    ./controls/resize \
    ./controls/save-changes \
    ./controls/start \
    ./controls/setup \
    ./controls/setup/parts \
    ./controls/setup/parts/font \
    ./controls/setup/parts/image \
    ./controls/setup/parts/matrix \
    ./controls/setup/parts/prepare \
    ./controls/setup/parts/reordering \
    ./controls/setup/parts/templates \
    ./controls/updates \
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
