/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "imageexport.h"
#include <QImage>
#include <QFileDialog>
#include <QStringList>
#include "idocument.h"
#include "datacontainer.h"
#include "filedialogoptions.h"

namespace Operations
{

ImageExport::ImageExport(QWidget *parentWidget, QObject *parent) :
  QObject(parent)
{
  this->mParentWidget = parentWidget;
  this->mExportIndex = 0;
}

bool ImageExport::prepare(const Data::Containers::IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)

  QFileDialog dialog(this->mParentWidget);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setDirectory(Settings::FileDialogOptions::directory(Settings::FileDialogOptions::Dialogs::ExportImage));
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setWindowTitle(tr("Save image file"));

  QStringList filters;
  filters << "Windows Bitmap (*.bmp)" // Nothing to translate
          << "Joint Photographic Experts Group (*.jpg *.jpeg)"
          << "Portable Network Graphics (*.png)"
          << "Portable Pixmap (*.ppm)"
          << "Tagged Image File Format (*.tiff)"
          << "X11 Bitmap (*.xbm)"
          << "X11 Bitmap (*.xpm)";
  dialog.setNameFilters(filters);
  dialog.selectNameFilter(
    filters.at(
      Settings::FileDialogOptions::filterIndex(Settings::FileDialogOptions::Dialogs::ExportImage) < filters.count()
      ?
      Settings::FileDialogOptions::filterIndex(Settings::FileDialogOptions::Dialogs::ExportImage)
      :
      0));

  if (dialog.exec() == QDialog::Accepted) {
    QString filter = dialog.selectedNameFilter();
    Settings::FileDialogOptions::setDirectory(Settings::FileDialogOptions::Dialogs::ExportImage, dialog.directory().absolutePath());
    Settings::FileDialogOptions::setFilterIndex(Settings::FileDialogOptions::Dialogs::ExportImage, filters.indexOf(filter));
    QString ext = "png";

    if (filter.contains("bmp")) {
      ext = "bmp";
    } else if (filter.contains("jpg")) {
      ext = "jpg";
    } else if (filter.contains("png")) {
      ext = "png";
    } else if (filter.contains("ppm")) {
      ext = "ppm";
    } else if (filter.contains("tiff")) {
      ext = "tiff";
    } else if (filter.contains("xbm")) {
      ext = "xbm";
    } else if (filter.contains("xpm")) {
      ext = "xpm";
    }

    QString filename = dialog.selectedFiles().at(0);
    this->prepareFilenames(keys, filename, ext);

    this->mExportIndex = 0;

    return (this->mExportFilenames.length() > 0);
  }

  return false;
}

void ImageExport::applyDocument(Data::Containers::IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageExport::applyItem(Data::Containers::IDocument *doc, const QString &itemKey)
{
  if (this->mExportFilenames.length() == 1) {
    doc->dataContainer()->image(itemKey)->save(this->mExportFilenames.at(0));
  } else if (this->mExportFilenames.length() > 1) {
    if (this->mExportIndex < this->mExportFilenames.length()) {
      doc->dataContainer()->image(itemKey)->save(this->mExportFilenames.at(this->mExportIndex++));
    }
  }
}

void ImageExport::prepareFilenames(const QStringList &keys, const QString &filename, const QString &ext)
{
  this->mExportFilenames.clear();

  if (keys.length() >= 1) {
    QString filename2 = filename;

    QFileInfo info(filename2);

    QString fileExt = info.suffix().toLower();

    if (fileExt.isEmpty() || fileExt != ext) {
      filename2 += "." + ext;
    }

    if (keys.length() > 1) {
      filename2.chop(ext.length() + 1);

      int counter = 0;
      QStringListIterator iterator(keys);

      while (iterator.hasNext()) {
        QString key = iterator.next();

        QString filename3 = filename2;
        filename3 += ".";
        filename3 += QString("%1").arg(counter++, 10, 10, QChar('0'));
        filename3 += ".";
        filename3 += ext;

        this->mExportFilenames.append(filename3);
      }
    } else {
      this->mExportFilenames.append(filename2);
    }
  }
}

}
