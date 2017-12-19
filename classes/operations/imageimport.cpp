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

#include "imageimport.h"
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include "idocument.h"
#include "datacontainer.h"
#include "filedialogoptions.h"

namespace Operations
{

ImageImport::ImageImport(QWidget *parentWidget, QObject *parent) :
  QObject(parent)
{
  this->mParentWidget = parentWidget;
  this->mImportIndex = 0;
}

bool ImageImport::prepare(const Data::Containers::IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)

  QFileDialog dialog(this->mParentWidget);
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(Settings::FileDialogOptions::directory(Settings::FileDialogOptions::Dialogs::ImportImage));
  dialog.setFileMode(QFileDialog::ExistingFiles);
  dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
  dialog.setWindowTitle(tr("Open image file"));

  if (dialog.exec() == QDialog::Accepted) {
    Settings::FileDialogOptions::setDirectory(Settings::FileDialogOptions::Dialogs::ImportImage, dialog.directory().absolutePath());
    this->mSelectedFiles = dialog.selectedFiles();

    if (this->mSelectedFiles.length() > 0) {
      this->mImportIndex = 0;

      // check for items-files count equals
      if (this->mSelectedFiles.length() != keys.length()) {
        QString msg = tr("Selected %1 file(s) and %2 character(s).\nOnly minimal amount (%3) will be imported.").\
                      arg(this->mSelectedFiles.length()).\
                      arg(keys.length()).\
                      arg(qMin(this->mSelectedFiles.length(), keys.length()));

        QMessageBox box(this->mParentWidget);
        box.setIcon(QMessageBox::Warning);
        box.setInformativeText(msg);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setText(tr("Selected a different number of files and characters."));
        box.setWindowTitle(tr("Warning"));

        if (box.exec() != QMessageBox::Ok) {
          return false;
        }
      }

      return true;
    }
  }

  return false;
}

void ImageImport::applyDocument(Data::Containers::IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageImport::applyItem(Data::Containers::IDocument *doc, const QString &itemKey)
{
  if (this->mSelectedFiles.length() == 1) {
    QImage imageLoaded;
    imageLoaded.load(this->mSelectedFiles.at(0));
    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

    doc->dataContainer()->setImage(itemKey, &imageConverted);
  } else if (this->mSelectedFiles.length() > 1) {
    if (this->mImportIndex < this->mSelectedFiles.length()) {
      QImage imageLoaded;
      imageLoaded.load(this->mSelectedFiles.at(this->mImportIndex++));
      QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

      doc->dataContainer()->setImage(itemKey, &imageConverted);
    }
  }
}

}
