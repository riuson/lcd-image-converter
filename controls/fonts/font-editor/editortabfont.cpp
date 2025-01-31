/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#include "editortabfont.h"

#include "ui_editortabfont.h"

#include <QBuffer>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QSplitter>
#include <QTextStream>
#include <QWheelEvent>

#include "datacontainer.h"
#include "dialogfontchanged.h"
#include "editor.h"
#include "fontdocument.h"
#include "fonteditoroptions.h"
#include "fontparameters.h"
#include "imagesmodel.h"
#include "imagesscaledproxy.h"
#include "parser.h"
#include "statusdata.h"
#include "tagslist.h"

namespace AppUI
{
namespace Fonts
{

EditorTabFont::EditorTabFont(QWidget* parent) : QWidget(parent), ui(new Ui::EditorTabFont)
{
  ui->setupUi(this);

  this->mSplitter = new QSplitter(this);
  this->ui->horizontalLayout->addWidget(this->mSplitter);

  this->mDocument = new Data::Containers::FontDocument(this);

  this->mLastImagesCount = 0;

  this->mModel = new Data::Models::ImagesModel(this->mDocument->dataContainer(), this);

  this->mScaledProxy = new Data::Models::ImagesScaledProxy(this);
  this->mScaledProxy->setScale(Settings::FontEditorOptions::scale());
  this->connect(this->mScaledProxy, SIGNAL(scaleChanged(int)), SLOT(resizeToContents()));

  this->mScaledProxy->setSourceModel(this->mModel);

  this->ui->tableViewCharacters->setModel(this->mScaledProxy);

  QItemSelectionModel* selectionModel = this->ui->tableViewCharacters->selectionModel();
  this->connect(selectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(currentChanged(QModelIndex, QModelIndex)));

  this->mEditorObject = new ImageEditor::Editor(this);
  this->mEditorWidget = this->mEditorObject->widget();
  this->mSplitter->addWidget(this->mEditorWidget);
  this->mSplitter->addWidget(this->ui->tableViewCharacters);
  this->mSplitter->setChildrenCollapsible(false);

  this->connect(this->mDocument, SIGNAL(documentChanged()), SLOT(mon_documentChanged()));
  this->connect(this->mEditorObject, SIGNAL(imageChanged(const QImage*)), SLOT(mon_editor_imageChanged(const QImage*)));
  this->connect(this->mEditorObject, SIGNAL(mouseMoved(const QPoint*)), SLOT(mon_editor_mouseMove(const QPoint*)));
  this->connect(this->mEditorObject, SIGNAL(scaleChanged(int)), SLOT(mon_editor_scaleChanged(int)));

  this->ui->tableViewCharacters->resizeColumnsToContents();

  this->initStatusData();
}

EditorTabFont::~EditorTabFont()
{
  this->mEditorWidget->setParent(nullptr);
  delete ui;
  delete this->mScaledProxy;
  delete this->mModel;
  delete this->mEditorObject;
}

Data::Containers::IDocument* EditorTabFont::document() const
{
  return qobject_cast<Data::Containers::IDocument*>(this->mDocument);
}

QStringList EditorTabFont::selectedKeys() const
{
  QStringList result;

  QItemSelectionModel* selectionModel = this->ui->tableViewCharacters->selectionModel();

  if (selectionModel->hasSelection()) {
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for (int i = 0; i < indexes.length(); i++) {
      QVariant var = this->mModel->data(indexes.at(i), Data::Models::ImagesModel::KeyRole);
      QString key = var.toString();

      if (!result.contains(key)) {
        result << key;
      }
    }
  }

  return result;
}

AppUI::Status::StatusData* EditorTabFont::statusData() const { return this->mStatusData; }

IEditor::EditorType EditorTabFont::type() const { return EditorFont; }

void EditorTabFont::setFontCharacters(const QString& chars, const Data::Containers::FontParameters& parameters)
{
  this->mDocument->setFontCharacters(chars, parameters);

  this->updateTableFont();
  this->mModel->callReset();
  this->ui->tableViewCharacters->resizeColumnsToContents();

  this->mEditorObject->setImage(this->mDocument->dataContainer()->image(this->currentKey()));

  this->resizeToContents();
  this->updateStatus();
}

void EditorTabFont::fontCharacters(QString* chars, Data::Containers::FontParameters* parameters)
{
  this->mDocument->fontCharacters(chars, parameters);
}

void EditorTabFont::changeEvent(QEvent* e)
{
  QWidget::changeEvent(e);

  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;

    default:
      break;
  }
}

void EditorTabFont::wheelEvent(QWheelEvent* event)
{
  if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
    QPoint point = event->globalPosition().toPoint();
    point = this->mapFromGlobal(point);

    QRect tableRect = this->ui->tableViewCharacters->rect();
    QPoint tablePoint = this->ui->tableViewCharacters->pos();
    tableRect.moveTo(tablePoint);

    if (tableRect.contains(point.x(), point.y())) {
      if (qAbs(event->angleDelta().x()) < qAbs(event->angleDelta().y())) {
        if (event->angleDelta().y() > 0) {
          this->mScaledProxy->setScale(this->mScaledProxy->scale() + 1);
        } else if (event->angleDelta().y() < 0) {
          this->mScaledProxy->setScale(this->mScaledProxy->scale() - 1);
        }

        Settings::FontEditorOptions::setScale(this->mScaledProxy->scale());
      }

      event->accept();
    }
  }
}

void EditorTabFont::initStatusData()
{
  this->mStatusData = new AppUI::Status::StatusData(this);
  this->connect(this->mStatusData, SIGNAL(changed()), SIGNAL(statusChanged()));

  this->updateStatus();
}

void EditorTabFont::updateStatus()
{
  const QImage* currentImage = this->mDocument->dataContainer()->image(this->currentKey());
  this->mStatusData->setData(AppUI::Status::StatusData::ImageSize, QVariant(currentImage->size()));

  this->mStatusData->setData(AppUI::Status::StatusData::Scale, QVariant(this->mEditorObject->scale()));

  // status update: current image index
  {
    int current = this->mDocument->dataContainer()->keys().indexOf(this->currentKey());
    int total = this->mDocument->dataContainer()->count();

    this->mStatusData->setData(AppUI::Status::StatusData::ImagesCount, total);
    this->mStatusData->setData(AppUI::Status::StatusData::ImageIndex, current);
  }
}

QString EditorTabFont::currentKey() const
{
  QString result;

  QItemSelectionModel* selectionModel = this->ui->tableViewCharacters->selectionModel();

  if (selectionModel->hasSelection()) {
    QModelIndex currentIndex = selectionModel->currentIndex();

    if (currentIndex.isValid()) {
      QVariant var = this->mModel->data(currentIndex, Data::Models::ImagesModel::KeyRole);
      result = var.toString();
    } else {
    }
  }

  if (result.isEmpty()) {
    if (this->mDocument->dataContainer()->count() > 0) {
      result = this->mDocument->dataContainer()->keys().at(0);
    } else {
      result = "default";
    }
  }

  return result;
}

void EditorTabFont::updateSelectedImage()
{
  QItemSelectionModel* selectionModel = this->ui->tableViewCharacters->selectionModel();

  if (selectionModel->hasSelection()) {
    QModelIndex index = selectionModel->currentIndex();

    QVariant var = this->mModel->data(index, Data::Models::ImagesModel::ImageRole);

    if (var.isValid()) {
      QImage image = var.value<QImage>();
      this->mEditorObject->setImage(&image);

      this->updateStatus();
    }
  }
}

void EditorTabFont::mon_documentChanged()
{
  this->updateSelectedImage();
  emit this->documentChanged();

  if (this->mDocument->dataContainer()->count() != this->mLastImagesCount) {
    this->mon_documentChangedSignificantly();
    this->mLastImagesCount = this->mDocument->dataContainer()->count();
  }

  this->resizeToContents();
}

void EditorTabFont::mon_documentChangedSignificantly()
{
  this->mModel->callReset();
  this->updateTableFont();
  this->updateStatus();
  emit this->documentChanged();

  if (this->mDocument->dataContainer()->count() > 0) {
    QString key = this->currentKey();
    const QImage* image = this->mDocument->dataContainer()->image(key);
    this->mEditorObject->setImage(image);
  } else {
    QImage empty = QImage();
    this->mEditorObject->setImage(&empty);
  }
}

void EditorTabFont::mon_editor_imageChanged(const QImage* value)
{
  this->mDocument->dataContainer()->setImage(this->currentKey(), value);
}

void EditorTabFont::mon_editor_mouseMove(const QPoint* point)
{
  if (point->x() >= 0 && point->y() >= 0) {
    this->mStatusData->setData(AppUI::Status::StatusData::MouseCoordinates, QVariant(*point));
  } else {
    this->mStatusData->removeData(AppUI::Status::StatusData::MouseCoordinates);
  }
}

void EditorTabFont::mon_editor_scaleChanged(int scale)
{
  this->mStatusData->setData(AppUI::Status::StatusData::Scale, QVariant(scale));
}

void EditorTabFont::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  Q_UNUSED(previous);

  if (current.isValid()) {
    QVariant var = this->mModel->data(current, Data::Models::ImagesModel::KeyRole);
    QString key = var.toString();

    const QImage* image = this->mDocument->dataContainer()->image(key);
    this->mEditorObject->setImage(image);

    // update selection for currentKey() method, called by updateStatus()
    QItemSelectionModel* selectionModel = this->ui->tableViewCharacters->selectionModel();
    selectionModel->setCurrentIndex(current, QItemSelectionModel::SelectCurrent);

    this->updateStatus();
  }
}

void EditorTabFont::updateTableFont() { this->resizeToContents(); }

void EditorTabFont::resizeToContents()
{
  this->ui->tableViewCharacters->resizeColumnsToContents();
  this->ui->tableViewCharacters->resizeRowsToContents();
}

} // namespace Fonts
} // namespace AppUI

/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="font" name="Font">
    <family>Ubuntu</family>
    <size>12</size>
    <style>Normal</style>
    <widthType>proportional</widthType>
    <antialiasing>false</antialiasing>
    <string>
!"#$%&amp;'()*+,-./0123456789:;&lt;=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВ</string>
    <converted>/tmp/font.c</converted>
    <chars>
        <char character=" " code="0020">
            <picture
format="png">iVBORw0KGgoAAAANSUhEUgAAAAQAAAATCAIAAAA4QDsKAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAFUlEQVQImWP8//8/AwwwMSCBYc0BAO+LAyPoIK0eAAAAAElFTkSuQmCC</picture>
        </char>
    </chars>
</data>
 */
