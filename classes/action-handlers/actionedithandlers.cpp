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

#include "actionedithandlers.h"
#include "qt-version-check.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 0, 0)
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif // QT_VERSION

#include <QFileDialog>
#include <QTextStream>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>
#include "editortabimage.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include "parser.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "datacontainer.h"
#include "idocument.h"

ActionEditHandlers::ActionEditHandlers(QObject *parent) :
  ActionHandlersBase(parent)
{
}

void ActionEditHandlers::undo_triggered()
{
  if (this->editor() != NULL) {
    this->editor()->document()->undo();
  }
}

void ActionEditHandlers::redo_triggered()
{
  if (this->editor() != NULL) {
    this->editor()->document()->redo();
  }
}

void ActionEditHandlers::copy_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    if (keys.length() > 0) {
      QString key = keys.at(0);

      if (keys.length() > 1) {
        QString message = tr("More than 1 image(s) selected. But only one will be copied - \"%1\".", "Warning about image copy").arg(key);

        QMessageBox msgBox(this->mMainWindow->parentWidget());
        msgBox.setTextFormat(Qt::PlainText);
        msgBox.setWindowTitle(tr("Copy - Attention"));
        msgBox.setText(message);
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        if (msgBox.exec() != QMessageBox::Ok) {
          return;
        }
      }

      const QImage *image = this->editor()->document()->dataContainer()->image(key);

      QClipboard *clipboard = QApplication::clipboard();
      clipboard->setImage(*image);
    }
  }
}

void ActionEditHandlers::paste_triggered()
{
  if (this->editor() != NULL) {
    QClipboard *clipboard = QApplication::clipboard();

    if (clipboard->mimeData()->hasImage()) {
      QImage image = clipboard->image();

      QStringList keys = this->editor()->selectedKeys();

      if (keys.length() > 0) {
        if (keys.length() > 1) {
          QString message = tr("More than 1 image(s) selected. All of them will be overwritten.", "Warning about image paste");

          QMessageBox msgBox(this->mMainWindow->parentWidget());
          msgBox.setTextFormat(Qt::PlainText);
          msgBox.setWindowTitle(tr("Paste - Attention"));
          msgBox.setText(message);
          msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
          msgBox.setDefaultButton(QMessageBox::Cancel);

          if (msgBox.exec() != QMessageBox::Ok) {
            return;
          }
        }

        this->editor()->document()->beginChanges();

        QStringListIterator iterator(keys);

        while (iterator.hasNext()) {
          QString key = iterator.next();
          this->editor()->document()->dataContainer()->setImage(key, &image);
        }

        this->editor()->document()->endChanges(false);
      }
    }
  }
}

