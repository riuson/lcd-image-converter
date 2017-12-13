/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "actionfilehandlers.h"

#include "editortabimage.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QInputDialog>
#include <QLineEdit>
#include <QMap>
#include <QStringList>
#include "parser.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "datacontainer.h"
#include "idocument.h"
#include "preset.h"
#include "fontparameters.h"
#include "filedialogoptions.h"

namespace AppUI
{
namespace MenuHandlers
{

ActionFileHandlers::ActionFileHandlers(QObject *parent) :
  ActionHandlersBase(parent)
{
}

void ActionFileHandlers::newImage_triggered()
{
  bool ok;
  QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                       tr("Enter image name"),
                                       tr("Image name:"),
                                       QLineEdit::Normal,
                                       QString("Image"),
                                       &ok);

  if (ok) {
    EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
    this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

    emit this->tabCreated(ed);

    name = this->mMainWindow->findAvailableName(name);
    ed->document()->setDocumentName(name);
  }
}

void ActionFileHandlers::newFont_triggered()
{
  bool ok;
  QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                       tr("Enter font name"),
                                       tr("Font name:"),
                                       QLineEdit::Normal,
                                       QString("Font"),
                                       &ok);

  if (ok) {
    DialogFontSelect dialog(this->mMainWindow->parentWidget());

    if (dialog.exec() == QDialog::Accepted) {
      EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
      this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

      ed->document()->beginChanges();

      QString chars = dialog.characters();
      Data::Containers::FontParameters parameters;
      dialog.getFontParameters(&parameters);

      ed->setFontCharacters(chars,
                            parameters);

      emit this->tabCreated(ed);

      name = this->mMainWindow->findAvailableName(name);
      ed->document()->setDocumentName(name);

      ed->document()->endChanges(true);
      ed->document()->dataContainer()->historyInit();
    }
  }
}

void ActionFileHandlers::open_triggered()
{
  QFileDialog dialog(this->mMainWindow->parentWidget());
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(FileDialogOptions::directory(FileDialogOptions::Dialogs::OpenDocument));
  dialog.setFileMode(QFileDialog::ExistingFiles);
  dialog.setWindowTitle(tr("Open xml or image file"));

  QStringList filters;
  filters << tr("XML Files (*.xml)")
          << tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
  dialog.setNameFilters(filters);
  dialog.selectNameFilter(
    filters.at(
      FileDialogOptions::filterIndex(FileDialogOptions::Dialogs::OpenDocument) < filters.count()
      ?
      FileDialogOptions::filterIndex(FileDialogOptions::Dialogs::OpenDocument)
      :
      0));

  if (dialog.exec() == QDialog::Accepted) {
    QStringList filenames = dialog.selectedFiles();
    FileDialogOptions::setDirectory(FileDialogOptions::Dialogs::OpenDocument, dialog.directory().absolutePath());
    FileDialogOptions::setFilterIndex(FileDialogOptions::Dialogs::OpenDocument, filters.indexOf(dialog.selectedNameFilter()));

    this->openFiles(filenames);
  }
}

void ActionFileHandlers::rename_triggered()
{
  bool ok;
  IEditor *editor = this->editor();

  if (editor != nullptr) {
    QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                         tr("Rename"),
                                         tr("New name:"),
                                         QLineEdit::Normal,
                                         editor->document()->documentName(),
                                         &ok);

    if (ok) {
      editor->document()->setDocumentName(name);
    }
  }
}

void ActionFileHandlers::save_triggered()
{
  IEditor *editor = this->editor();

  if (editor != nullptr) {
    if (QFile::exists(editor->document()->documentFilename())) {
      editor->document()->save(editor->document()->documentFilename());
    } else {
      this->saveAs_triggered();
    }
  }
}

void ActionFileHandlers::saveAs_triggered()
{
  IEditor *editor = this->editor();

  if (editor != nullptr) {
    QFileDialog dialog(this->mMainWindow->parentWidget());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("XML Files (*.xml)"));
    dialog.setDefaultSuffix(QString("xml"));
    dialog.setWindowTitle(tr("Save file as"));

    QFileInfo outputFile(editor->document()->documentFilename());

    if (outputFile.exists()) {
      dialog.setDirectory(outputFile.dir());
    } else {
      dialog.setDirectory(FileDialogOptions::directory(FileDialogOptions::Dialogs::SaveDocument));
    }

    if (editor->document()->documentFilename().isEmpty()) {
      dialog.selectFile(editor->document()->documentName());
    } else {
      dialog.selectFile(editor->document()->documentFilename());
    }

    if (dialog.exec() == QDialog::Accepted) {
      FileDialogOptions::setDirectory(FileDialogOptions::Dialogs::SaveDocument, dialog.directory().absolutePath());
      QString filename = dialog.selectedFiles().at(0);
      editor->document()->save(filename);

      emit this->rememberFilename(filename);
    }
  }
}

void ActionFileHandlers::close_triggered()
{
  emit closeRequest(this->mMainWindow->currentTab());
}

void ActionFileHandlers::closeAll_triggered()
{
  QList<QWidget *> list;

  this->mMainWindow->tabsList(&list);

  for (int i = 0; i < list.count(); i++) {
    emit closeRequest(list.at(i));
  }
}

void ActionFileHandlers::convert_triggered()
{
  IEditor *editor = this->mMainWindow->currentEditor();

  if (editor != nullptr) {
    this->convertDocument(editor->document(), true);
  }
}

void ActionFileHandlers::convertAll_triggered()
{
  QList<QWidget *> list;

  this->mMainWindow->tabsList(&list);

  for (int i = 0; i < list.count(); i++) {
    IEditor *editor = dynamic_cast<IEditor *> (list.at(i));

    if (editor != nullptr) {
      this->convertDocument(editor->document(), false);
    }
  }
}

void ActionFileHandlers::openFiles(const QStringList &filenames)
{
  QList<QWidget *> existingTabs;
  this->mMainWindow->tabsList(&existingTabs);
  QMap<QString, QWidget *> existingFilesInEditors;
  QString lastExistingFile;

  for (int i = 0; i < existingTabs.count(); i++) {
    IEditor *editor = dynamic_cast<IEditor *> (existingTabs.at(i));

    if (editor != nullptr) {
      existingFilesInEditors.insert(editor->document()->documentFilename(), existingTabs.at(i));
    }
  }

  // binary image
  QStringList filesBinaryImage;
  {
    QStringList imageExtensions;
    imageExtensions << "bmp" << "gif" << "jpg" << "jpeg" << "png" << "pbm" << "pgm" << "ppm" << "tiff" << "xbm" << "xpm";

    foreach (const QString &filename, filenames) {
      QFileInfo info(filename);

      if (info.exists() && imageExtensions.contains(info.suffix().toLower())) {
        filesBinaryImage << filename;
      }
    }
  }

  // document image
  QStringList filesDocumentImage;
  {
    foreach (const QString &filename, filenames) {
      QFileInfo info(filename);

      if (info.exists() && info.suffix().toLower() == "xml" && !existingFilesInEditors.contains(filename)) {
        QFile file(filename);

        if (file.open(QIODevice::ReadWrite)) {
          QTextStream stream(&file);
          QRegExp regImage("<data.+type=\"image\"", Qt::CaseInsensitive);

          while (!stream.atEnd()) {
            QString readedLine = stream.readLine();

            if (readedLine.contains(regImage)) {
              filesDocumentImage << filename;
              break;
            }
          }

          file.close();
        }
      } else if (existingFilesInEditors.keys().contains(filename)) {
        lastExistingFile = filename;
      }
    }
  }

  // document font
  QStringList filesDocumentFont;
  {
    foreach (const QString &filename, filenames) {
      QFileInfo info(filename);

      if (info.exists() && info.suffix().toLower() == "xml" && !existingFilesInEditors.contains(filename)) {
        QFile file(filename);

        if (file.open(QIODevice::ReadWrite)) {
          QTextStream stream(&file);
          QRegExp regFont("<data.+type=\"font\"", Qt::CaseInsensitive);

          while (!stream.atEnd()) {
            QString readedLine = stream.readLine();

            if (readedLine.contains(regFont)) {
              filesDocumentFont << filename;
              break;
            }
          }

          file.close();
        }
      } else if (existingFilesInEditors.keys().contains(filename)) {
        lastExistingFile = filename;
      }
    }
  }

  this->openBinaryImage(filesBinaryImage);
  this->openImage(filesDocumentImage);
  this->openFont(filesDocumentFont);

  if (existingFilesInEditors.contains(lastExistingFile)) {
    this->tabSelect(existingFilesInEditors.value(lastExistingFile));
  }
}

void ActionFileHandlers::openFile(const QString &filename)
{
  QStringList files = QStringList() << filename;
  this->openFiles(files);
}

void ActionFileHandlers::openImage(QImage *image, const QString &documentName)
{
  EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
  this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

  QString name = this->mMainWindow->findAvailableName(documentName);

  // assign image
  {
    QStringList keys = ed->selectedKeys();

    QStringListIterator iterator(keys);

    while (iterator.hasNext()) {
      QString key = iterator.next();
      ed->document()->dataContainer()->setImage(key, image);
    }
  }

  ed->document()->setDocumentName(name);

  emit this->tabCreated(ed);
}

void ActionFileHandlers::openBinaryImage(const QStringList &filenames)
{
  foreach (const QString &filename, filenames) {
    QFileInfo info(filename);

    QImage imageLoaded;

    if (info.exists() && imageLoaded.load(filename)) {
      QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

      EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
      this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

      QString name = this->mMainWindow->findAvailableName(info.baseName());

      // assign image
      {
        QStringList keys = ed->selectedKeys();

        QStringListIterator iterator(keys);

        while (iterator.hasNext()) {
          QString key = iterator.next();
          ed->document()->dataContainer()->setImage(key, &imageConverted);
        }
      }

      emit this->tabCreated(ed);
      ed->document()->setDocumentName(name);
    }
  }
}

void ActionFileHandlers::openImage(const QStringList &filenames)
{
  foreach (const QString &filename, filenames) {
    EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
    this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

    emit this->tabCreated(ed);
    ed->document()->load(filename);
    emit this->rememberFilename(filename);
  }
}

void ActionFileHandlers::openFont(const QStringList &filenames)
{
  foreach (const QString &filename, filenames) {
    EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
    this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

    emit this->tabCreated(ed);
    ed->document()->load(filename);
    emit this->rememberFilename(filename);
  }
}

void ActionFileHandlers::convertDocument(Data::Containers::IDocument *document, bool request)
{
  // converter output file name
  QString outputFileName = document->outputFilename();

  // document file name
  QString documentFileName = document->documentFilename();

  // if file name not specified, show dialog
  bool outputFilenameNotSpecified = outputFileName.isEmpty();

  // if document not saved, do not save output file name
  bool documentFilenameNotSpecified = documentFileName.isEmpty();

  // show dialog
  if (request || outputFilenameNotSpecified) {
    QFileDialog dialog(qobject_cast<QWidget *>(this->parent()));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile(outputFileName);
    dialog.setDirectory(FileDialogOptions::directory(FileDialogOptions::Dialogs::ConvertDocument));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDefaultSuffix(QString("c"));
    dialog.setWindowTitle(tr("Save result file as"));

    QStringList filters;
    filters << tr("C source code (*.c)")
            << tr("C++ source code (*.cpp)")
            << tr("C/C++ headers (*.h)")
            << tr("C/C++ files (*.c *.cpp *.h)")
            << tr("All Files (*.*)");
    dialog.setNameFilters(filters);
    dialog.selectNameFilter(
      filters.at(
        FileDialogOptions::filterIndex(FileDialogOptions::Dialogs::ConvertDocument) < filters.count()
        ?
        FileDialogOptions::filterIndex(FileDialogOptions::Dialogs::ConvertDocument)
        :
        0));

    if (outputFilenameNotSpecified) {
      dialog.selectFile(document->documentName());
    } else {
      dialog.selectFile(outputFileName);
    }

    if (dialog.exec() == QDialog::Accepted) {
      outputFileName = dialog.selectedFiles().at(0);
      FileDialogOptions::setDirectory(FileDialogOptions::Dialogs::ConvertDocument, dialog.directory().absolutePath());
      FileDialogOptions::setFilterIndex(FileDialogOptions::Dialogs::ConvertDocument, filters.indexOf(dialog.selectedNameFilter()));
    } else {
      outputFileName = "";
    }
  }

  // if file name specified, save result
  if (!outputFileName.isEmpty()) {
    QFile file(outputFileName);

    if (file.open(QFile::WriteOnly)) {
      Preset preset;
      preset.load(Preset::selectedName());

      QString result = document->convert(&preset);

      file.write(result.toUtf8());
      file.close();

      if ((document->outputFilename() != outputFileName) && !documentFilenameNotSpecified) {
        document->setOutputFilename(outputFileName);
      }
    }
  }
}

void ActionFileHandlers::documentChanged()
{
  QWidget *w = qobject_cast<QWidget *>(sender());
  emit this->tabChanged(w);
}

} // namespace MenuHandlers
} // namespace AppUI
