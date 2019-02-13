# -------------------------------------------------
# Project created by QtCreator 2010-06-22T13:10:53
# -------------------------------------------------
OUTDIR              = ./

CONFIG(debug, debug|release) {
    OUTDIR          = $$OUTDIR/debug
} else {
    OUTDIR          = $$OUTDIR/release
}
unix:OUTDIR         = $$OUTDIR/linux
win32:OUTDIR        = $$OUTDIR/windows

OBJECTS_DIR         = $$OUTDIR/.obj
MOC_DIR             = $$OUTDIR/.moc
UI_DIR              = $$OUTDIR/.uic
RCC_DIR             = $$OUTDIR/.rcc

QT += xml xmlpatterns network svg qml widgets
TARGET = lcd-image-converter
TEMPLATE = app

# Uncomment next line to enable console output on Windows.
#CONFIG += console
CONFIG+=c++11
QMAKE_CXXFLAGS += -std=c++11

DESTDIR             = $$OUTDIR/output
QMAKE_LIBDIR       += $$DESTDIR

# Qt version required >= 5.5, since 2017-10-05.
# Qt < 5.0
lessThan(QT_MAJOR_VERSION, 5) {
  error(Qt version required >= 5.5)
}
#Qt < 5.5
greaterThan(QT_MAJOR_VERSION, 4) {
  lessThan(QT_MINOR_VERSION, 5) {
    error(Qt version required >= 5.5)
  }
}

DEFINES += QT_MAJOR_VERSION="$$QT_MAJOR_VERSION"
DEFINES += QT_MINOR_VERSION="$$QT_MINOR_VERSION"
DEFINES += QT_PATCH_VERSION="$$QT_PATCH_VERSION"

CONFIG(debug, debug|release) {
    DEFINES        += DEBUG_VERSION
    TARGET          = $$join(TARGET,,,-debug)
}

SOURCES += main.cpp \
    classes/action-handlers/actionedithandlers.cpp \
    classes/action-handlers/actionfilehandlers.cpp \
    classes/action-handlers/actionfonthandlers.cpp \
    classes/action-handlers/actionhandlersbase.cpp \
    classes/action-handlers/actionhelphandlers.cpp \
    classes/action-handlers/actionimagehandlers.cpp \
    classes/action-handlers/actionsetuphandlers.cpp \
    classes/cmdline/cmdline.cpp \
    classes/cmdline/modeconvertfont.cpp \
    classes/cmdline/modeconvertimage.cpp \
    classes/cmdline/modehex2bin.cpp \
    classes/cmdline/modeparserbase.cpp \
    classes/compression/rlecompressor.cpp \
    classes/compression/rlesequence.cpp \
    classes/data/canvasmodinfo.cpp \
    classes/data/canvasmodproxy.cpp \
    classes/data/datacontainer.cpp \
    classes/data/fontdocument.cpp \
    classes/data/historykeeper.cpp \
    classes/data/historyrecord.cpp \
    classes/data/imagedocument.cpp \
    classes/data/imagesmodel.cpp \
    classes/data/imagesscaledproxy.cpp \
    classes/data/revisioninfo.cpp \
    classes/data/transposeproxy.cpp \
    classes/imageeditor/editor.cpp \
    classes/imageeditor/toolcolor.cpp \
    classes/imageeditor/toolfill.cpp \
    classes/imageeditor/toolline.cpp \
    classes/imageeditor/toolmove.cpp \
    classes/imageeditor/toolselect.cpp \
    classes/imageeditor/toolsmanager.cpp \
    classes/imageeditor/toolpen.cpp \
    classes/imageeditor/toolrect.cpp \
    classes/imageeditor/toolzoom.cpp \
    classes/operations/documentoperator.cpp \
    classes/operations/fontminimizeheight.cpp \
    classes/operations/fontresize.cpp \
    classes/operations/imageeditinexternaltool.cpp \
    classes/operations/imageexport.cpp \
    classes/operations/imageflip.cpp \
    classes/operations/imagegrayscale.cpp \
    classes/operations/imageimport.cpp \
    classes/operations/imageinverse.cpp \
    classes/operations/imageresize.cpp \
    classes/operations/imagerotate.cpp \
    classes/operations/imageshift.cpp \
    classes/parser/convert/bitmaphelper.cpp \
    classes/parser/convert/bitstream.cpp \
    classes/parser/convert/converterhelper.cpp \
    classes/parser/convert/convimagepixels.cpp \
    classes/parser/convert/convimagescan.cpp \
    classes/parser/convert/fonthelper.cpp \
    classes/parser/parsedimagedata.cpp \
    classes/parser/parser.cpp \
    classes/parser/tagslist.cpp \
    classes/settings/appsettings.cpp \
    classes/settings/conversionpreviewoptions.cpp \
    classes/settings/externaltooloptions.cpp \
    classes/settings/filedialogoptions.cpp \
    classes/settings/fonteditoroptions.cpp \
    classes/settings/imageeditoroptions.cpp \
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
    classes/settings/setupdialogoptions.cpp \
    classes/status/statusdata.cpp \
    classes/status/statusmanager.cpp \
    controls/about/dialogabout.cpp \
    controls/fonts/font-changed/dialogfontchanged.cpp \
    controls/fonts/font-editor/editortabfont.cpp \
    controls/fonts/font-new/charactersmodel.cpp \
    controls/fonts/font-new/dialogfontselect.cpp \
    controls/fonts/font-new/dialogfontselectdata.cpp \
    controls/fonts/font-new/unicodeblocksmodel.cpp \
    controls/fonts/font-new/unicodeblocksfiltermodel.cpp \
    controls/fonts/font-preview/dialogfontpreview.cpp \
    controls/fonts/font-range/dialogfontrange.cpp \
    controls/imageeditor/windoweditor.cpp \
    controls/images/editortabimage.cpp \
    controls/images/widgetbitmapeditor.cpp \
    controls/main/mainwindow.cpp \
    controls/main/revisionlabel.cpp \
    controls/resize/columnsreorderproxy.cpp \
    controls/resize/dialogcanvasresize.cpp \
    controls/resize/imagesfilterproxy.cpp \
    controls/resize/imagesresizedproxy.cpp \
    controls/save-changes/dialogsavechanges.cpp \
    controls/setup/dialogexternaleditor.cpp \
    controls/setup/dialogoptions.cpp \
    controls/setup/dialogpreview.cpp \
    controls/setup/parts/font/setuptabfont.cpp \
    controls/setup/parts/image/setuptabimage.cpp \
    controls/setup/parts/matrix/matrixitemdelegate.cpp \
    controls/setup/parts/matrix/matrixpreviewmodel.cpp \
    controls/setup/parts/matrix/setuptabmatrix.cpp \
    controls/setup/parts/prepare/demogenerator.cpp \
    controls/setup/parts/prepare/preprocessing/setuptabpreparepreprocessing.cpp \
    controls/setup/parts/prepare/scanning/setuptabpreparescanning.cpp \
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
    classes/cmdline/cmdline.h \
    classes/cmdline/modeconvertfont.h \
    classes/cmdline/modeconvertimage.h \
    classes/cmdline/modehex2bin.h \
    classes/cmdline/modeparserbase.h \
    classes/compression/rlecompressor.h \
    classes/compression/rlesequence.h \
    classes/data/canvasmodinfo.h \
    classes/data/canvasmodproxy.h \
    classes/data/datacontainer.h \
    classes/data/fontdocument.h \
    classes/data/fontparameters.h \
    classes/data/historykeeper.h \
    classes/data/historyrecord.h \
    classes/data/imagedocument.h \
    classes/data/imagesmodel.h \
    classes/data/imagesscaledproxy.h \
    classes/data/revisioninfo.h \
    classes/data/transposeproxy.h \
    classes/imageeditor/editor.h \
    classes/imageeditor/iimageeditor.h \
    classes/imageeditor/iimageeditorparams.h \
    classes/imageeditor/iimageeditortool.h \
    classes/imageeditor/iimageselection.h \
    classes/imageeditor/toolcolor.h \
    classes/imageeditor/toolfill.h \
    classes/imageeditor/toolline.h \
    classes/imageeditor/toolmove.h \
    classes/imageeditor/toolselect.h \
    classes/imageeditor/toolsmanager.h \
    classes/imageeditor/toolpen.h \
    classes/imageeditor/toolrect.h \
    classes/imageeditor/toolzoom.h \
    classes/operations/documentoperator.h \
    classes/operations/fontminimizeheight.h \
    classes/operations/fontresize.h \
    classes/operations/imageeditinexternaltool.h \
    classes/operations/imageexport.h \
    classes/operations/imageflip.h \
    classes/operations/imagegrayscale.h \
    classes/operations/imageimport.h \
    classes/operations/imageinverse.h \
    classes/operations/imageresize.h \
    classes/operations/imagerotate.h \
    classes/operations/imageshift.h \
    classes/operations/ioperation.h \
    classes/parser/convert/bitmaphelper.h \
    classes/parser/convert/bitstream.h \
    classes/parser/convert/conversion_options.h \
    classes/parser/convert/converterhelper.h \
    classes/parser/convert/convimagepixels.h \
    classes/parser/convert/convimagescan.h \
    classes/parser/convert/fonthelper.h \
    classes/parser/parsedimagedata.h \
    classes/parser/parser.h \
    classes/parser/tagslist.h \
    classes/settings/appsettings.h \
    classes/settings/conversionpreviewoptions.h \
    classes/settings/externaltooloptions.h \
    classes/settings/filedialogoptions.h \
    classes/settings/fonteditoroptions.h \
    classes/settings/imageeditoroptions.h \
    classes/settings/languageoptions.h \
    classes/settings/presets/fontoptions.h \
    classes/settings/presets/imageoptions.h \
    classes/settings/presets/ipresetsoptionspart.h \
    classes/settings/presets/matrixoptions.h \
    classes/settings/presets/prepareoptions.h \
    classes/settings/presets/preset.h \
    classes/settings/presets/reorderingoptions.h \
    classes/settings/presets/templateoptions.h \
    classes/settings/recentlist.h \
    classes/settings/resizesettings.h \
    classes/settings/setupdialogoptions.h \
    classes/status/statusdata.h \
    classes/status/statusmanager.h \
    controls/about/dialogabout.h \
    controls/fonts/font-changed/dialogfontchanged.h \
    controls/fonts/font-editor/editortabfont.h \
    controls/fonts/font-new/charactersmodel.h \
    controls/fonts/font-new/dialogfontselect.h \
    controls/fonts/font-new/dialogfontselectdata.h \
    controls/fonts/font-new/unicodeblocksmodel.h \
    controls/fonts/font-new/unicodeblocksfiltermodel.h \
    controls/fonts/font-preview/dialogfontpreview.h \
    controls/fonts/font-range/dialogfontrange.h \
    controls/imageeditor/windoweditor.h \
    controls/images/editortabimage.h \
    controls/images/widgetbitmapeditor.h \
    controls/main/mainwindow.h \
    controls/main/revisionlabel.h \
    controls/resize/columnsreorderproxy.h \
    controls/resize/dialogcanvasresize.h \
    controls/resize/imagesfilterproxy.h \
    controls/resize/imagesresizedproxy.h \
    controls/save-changes/dialogsavechanges.h \
    controls/setup/dialogexternaleditor.h \
    controls/setup/dialogoptions.h \
    controls/setup/dialogpreview.h \
    controls/setup/parts/font/setuptabfont.h \
    controls/setup/parts/image/setuptabimage.h \
    controls/setup/parts/matrix/matrixitemdelegate.h \
    controls/setup/parts/matrix/matrixpreviewmodel.h \
    controls/setup/parts/matrix/setuptabmatrix.h \
    controls/setup/parts/prepare/demogenerator.h \
    controls/setup/parts/prepare/preprocessing/setuptabpreparepreprocessing.h \
    controls/setup/parts/prepare/scanning/setuptabpreparescanning.h \
    controls/setup/parts/prepare/setuptabprepare.h \
    controls/setup/parts/reordering/reorderingitemdelegate.h \
    controls/setup/parts/reordering/reorderingpreviewmodel.h \
    controls/setup/parts/reordering/setuptabreordering.h \
    controls/setup/parts/templates/setuptabtemplates.h \
    controls/start/starttab.h \
    controls/updates/dialogupdates.h \
    interfaces/idocument.h \
    interfaces/ieditor.h \
    interfaces/imainwindow.h \
    qt-version-check.h

FORMS += \
    controls/about/dialogabout.ui \
    controls/fonts/font-changed/dialogfontchanged.ui \
    controls/fonts/font-editor/editortabfont.ui \
    controls/fonts/font-new/dialogfontselect.ui \
    controls/fonts/font-preview/dialogfontpreview.ui \
    controls/fonts/font-range/dialogfontrange.ui \
    controls/imageeditor/windoweditor.ui \
    controls/images/editortabimage.ui \
    controls/images/widgetbitmapeditor.ui \
    controls/main/mainwindow.ui \
    controls/resize/dialogcanvasresize.ui \
    controls/setup/dialogexternaleditor.ui \
    controls/setup/dialogoptions.ui \
    controls/setup/dialogpreview.ui \
    controls/setup/parts/font/setuptabfont.ui \
    controls/setup/parts/image/setuptabimage.ui \
    controls/setup/parts/matrix/setuptabmatrix.ui \
    controls/setup/parts/prepare/scanning/setuptabpreparescanning.ui \
    controls/setup/parts/prepare/setuptabprepare.ui \
    controls/setup/parts/prepare/preprocessing/setuptabpreparepreprocessing.ui \
    controls/setup/parts/reordering/setuptabreordering.ui \
    controls/setup/parts/templates/setuptabtemplates.ui \
    controls/start/starttab.ui \
    controls/updates/dialogupdates.ui

INCLUDEPATH += $$PWD \
    $$PWD/classes \
    $$PWD/classes/action-handlers \
    $$PWD/classes/compression \
    $$PWD/classes/data \
    $$PWD/classes/cmdline \
    $$PWD/classes/imageeditor \
    $$PWD/classes/operations \
    $$PWD/classes/status \
    $$PWD/classes/parser \
    $$PWD/classes/parser/convert \
    $$PWD/classes/preview-models \
    $$PWD/classes/settings \
    $$PWD/classes/settings/presets \
    $$PWD/controls \
    $$PWD/controls/about \
    $$PWD/controls/fonts/font-changed \
    $$PWD/controls/fonts/font-editor \
    $$PWD/controls/fonts/font-new \
    $$PWD/controls/fonts/font-preview \
    $$PWD/controls/fonts/font-range \
    $$PWD/controls/images \
    $$PWD/controls/imageeditor \
    $$PWD/controls/main \
    $$PWD/controls/resize \
    $$PWD/controls/save-changes \
    $$PWD/controls/start \
    $$PWD/controls/setup \
    $$PWD/controls/setup/parts \
    $$PWD/controls/setup/parts/font \
    $$PWD/controls/setup/parts/image \
    $$PWD/controls/setup/parts/matrix \
    $$PWD/controls/setup/parts/prepare \
    $$PWD/controls/setup/parts/prepare/scanning \
    $$PWD/controls/setup/parts/prepare/preprocessing \
    $$PWD/controls/setup/parts/reordering \
    $$PWD/controls/setup/parts/templates \
    $$PWD/controls/updates \
    $$PWD/interfaces

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
    readme.md \
    resources/history.xml \
    resources/history.xsl \
    resources/history.css \
    resources/unicode_blocks.txt \
    resources/script_top2bottom_forward.js \
    resources/scan_scripts/pixels_example.js \
    resources/scan_scripts/pixels_template.js \
    resources/scan_scripts/scan_template.js \
    resources/scan_scripts/scan_top2bottom_forward_band.js \
    resources/scan_scripts/scan_top2bottom_forward.js \
    resources/scan_scripts/scan_top2bottom_backward_band.js \
    resources/scan_scripts/scan_top2bottom_backward.js \
    resources/scan_scripts/scan_right2left_forward_band.js \
    resources/scan_scripts/scan_right2left_forward.js \
    resources/scan_scripts/scan_right2left_backward_band.js \
    resources/scan_scripts/scan_right2left_backward.js \
    resources/scan_scripts/scan_left2right_forward_band.js \
    resources/scan_scripts/scan_left2right_forward.js \
    resources/scan_scripts/scan_left2right_backward_band.js \
    resources/scan_scripts/scan_left2right_backward.js \
    resources/scan_scripts/scan_bottom2top_forward_band.js \
    resources/scan_scripts/scan_bottom2top_forward.js \
    resources/scan_scripts/scan_bottom2top_backward_band.js \
    resources/scan_scripts/scan_bottom2top_backward.js


# generate version info file on each build
# sh script ($1 - path to project's directory):
# git --git-dir $1/.git log --pretty=format:"#define GIT_REVISION \"%H\\0\" %n#define GIT_REVISION_ABBR \"%h\\0\" %n#define GIT_COMMIT_ADATE \"%ai\\0\" %n#define GIT_COMMIT_AT %at" -1 > $1/resources/revision.h
win32 {
  PERCENT = %%
}
unix {
  PERCENT = %
}
NEWLINE = $${PERCENT}n
VERSION_LOG_FORMAT = $${LITERAL_HASH}define GIT_REVISION \\\"$${PERCENT}H\\\"$${NEWLINE}$${LITERAL_HASH}define GIT_REVISION_ABBR \\\"$${PERCENT}h\\\"$${NEWLINE}$${LITERAL_HASH}define GIT_COMMIT_ADATE \\\"$${PERCENT}ai\\\"$${NEWLINE}$${LITERAL_HASH}define GIT_COMMIT_AT $${PERCENT}at$${NEWLINE}
version.target = git_revision
version.commands = git --git-dir $${PWD}/.git log --pretty=format:\"$${VERSION_LOG_FORMAT}\" -1 > $${PWD}/resources/revision.h

QMAKE_EXTRA_TARGETS += version
PRE_TARGETDEPS += git_revision

# compile translation
translation_ru.target = $$PWD/resources/lcd-image-converter-ru.qm
translation_ru.commands = lrelease $$PWD/resources/lcd-image-converter-ru.ts $$PWD/resources/lcd-image-converter-ru.qm

QMAKE_EXTRA_TARGETS += translation_ru
PRE_TARGETDEPS += $$PWD/resources/lcd-image-converter-ru.qm $$PWD/resources/lcd-image-converter-ru.ts

DISTFILES += \
    astyle.astylerc
