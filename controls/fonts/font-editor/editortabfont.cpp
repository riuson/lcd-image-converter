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
//-----------------------------------------------------------------------------
#include <QSplitter>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
#include <QPainter>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>
#include <QWheelEvent>

#include "widgetbitmapeditor.h"
#include "datacontainer.h"
#include "imagesmodel.h"
#include "parser.h"
#include "dialogfontchanged.h"
#include "tags.h"
#include "statusdata.h"
#include "fonteditoroptions.h"
#include "fontdocument.h"
//-----------------------------------------------------------------------------
EditorTabFont::EditorTabFont(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EditorTabFont)
{
    ui->setupUi(this);

    this->mSplitter = new QSplitter(this);
    this->ui->horizontalLayout->addWidget(this->mSplitter);

    this->mDocument = new FontDocument(this);

    this->mLastImagesCount = 0;

    this->mModel = new ImagesModel(this->mDocument->dataContainer(), Qt::Vertical, this);
    this->ui->tableViewCharacters->setModel(this->mModel);
    this->connect(this->mModel, SIGNAL(scaleChanged()), SLOT(resizeToContents()));
    this->mModel->setScale(FontEditorOptions::scale());

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    this->connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(currentChanged(QModelIndex,QModelIndex)));

    this->mEditor = new WidgetBitmapEditor(this);
    this->mSplitter->addWidget(this->mEditor);
    this->mSplitter->addWidget(this->ui->tableViewCharacters);
    this->mSplitter->setChildrenCollapsible(false);

    this->connect(this->mDocument, SIGNAL(documentChanged()), SLOT(mon_documentChanged()));
    this->connect(this->mEditor, SIGNAL(imageChanged()), SLOT(mon_editor_imageChanged()));
    this->connect(this->mEditor, SIGNAL(mouseMove(QPoint)), SLOT(mon_editor_mouseMove(QPoint)));
    this->connect(this->mEditor, SIGNAL(scaleSchanged(int)), SLOT(mon_editor_scaleChanged(int)));

    this->ui->tableViewCharacters->resizeColumnsToContents();

    this->initStatusData();
}
//-----------------------------------------------------------------------------
EditorTabFont::~EditorTabFont()
{
    delete ui;
    delete this->mModel;
}
//-----------------------------------------------------------------------------
IDocument *EditorTabFont::document() const
{
    return qobject_cast<IDocument *>(this->mDocument);
}
//-----------------------------------------------------------------------------
QStringList EditorTabFont::selectedKeys() const
{
    QStringList result;

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndexList indexes = selectionModel->selectedIndexes();
        for (int i = 0; i < indexes.length(); i++)
        {
            QVariant var = this->mModel->data(indexes.at(i), ImagesModel::KeyRole);
            QString key = var.toString();
            if (!result.contains(key))
            {
                result << key;
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
StatusData *EditorTabFont::statusData() const
{
    return this->mStatusData;
}
//-----------------------------------------------------------------------------
IEditor::EditorType EditorTabFont::type() const
{
    return EditorFont;
}
//-----------------------------------------------------------------------------
void EditorTabFont::setFontCharacters(const QString &chars,
                                      const QString &fontFamily,
                                      const bool &_bold,
                                      const bool &_italic,
                                      const int _size,
                                      const bool _monospaced,
                                      const bool _antialiasing)
{
    this->mDocument->setFontCharacters(
                chars,
                fontFamily,
                _bold,
                _italic,
                _size,
                _monospaced,
                _antialiasing);

    this->updateTableFont();
    this->mModel->callReset();
    this->ui->tableViewCharacters->resizeColumnsToContents();

    this->mEditor->setImage(this->mDocument->dataContainer()->image(this->currentKey()));

    this->resizeToContents();
    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabFont::fontCharacters(QString *chars,
                                   QString *fontFamily,
                                   bool *_bold,
                                   bool *_italic,
                                   int *_size,
                                   bool *_monospaced,
                                   bool *_antialiasing)
{
    this->mDocument->fontCharacters(
                chars,
                fontFamily,
                _bold,
                _italic,
                _size,
                _monospaced,
                _antialiasing);
}
//-----------------------------------------------------------------------------
void EditorTabFont::changeEvent(QEvent *e)
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
//-----------------------------------------------------------------------------
void EditorTabFont::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
    {
        QPoint point = event->globalPos();
        point = this->mapFromGlobal(point);

        QRect tableRect = this->ui->tableViewCharacters->rect();
        QPoint tablePoint = this->ui->tableViewCharacters->pos();
        tableRect.moveTo(tablePoint);

        if (tableRect.contains(point.x(), point.y()))
        {
            if (event->orientation() == Qt::Vertical)
            {
                if (event->delta() > 0)
                {
                    this->mModel->setScale(this->mModel->scale() + 1);
                }
                else if (event->delta() < 0)
                {
                    this->mModel->setScale(this->mModel->scale() - 1);
                }
                FontEditorOptions::setScale(this->mModel->scale());
            }
            event->accept();
        }
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::initStatusData()
{
    this->mStatusData = new StatusData(this);
    this->connect(this->mStatusData, SIGNAL(changed()), SIGNAL(statusChanged()));

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabFont::updateStatus()
{
    const QImage *currentImage = this->mDocument->dataContainer()->image(this->currentKey());
    this->mStatusData->setData(StatusData::ImageSize, QVariant(currentImage->size()));

    this->mStatusData->setData(StatusData::Scale, QVariant(this->mEditor->scale()));

    // status update: current image index
    {
        int current = this->mDocument->dataContainer()->keys().indexOf(this->currentKey());
        int total = this->mDocument->dataContainer()->count();

        this->mStatusData->setData(StatusData::ImagesCount, total);
        this->mStatusData->setData(StatusData::ImageIndex, current);
    }
}
//-----------------------------------------------------------------------------
QString EditorTabFont::currentKey() const
{
    QString result;

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndex currentIndex = selectionModel->currentIndex();
        if (currentIndex.isValid())
        {
            QVariant var = this->mModel->data(currentIndex, ImagesModel::KeyRole);
            result = var.toString();
        }
        else
        {
        }
    }

    if (result.isEmpty())
    {
        if (this->mDocument->dataContainer()->count() > 0)
        {
            result = this->mDocument->dataContainer()->keys().at(0);
        }
        else
        {
            result = "default";
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
void EditorTabFont::updateSelectedImage()
{
    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndex index = selectionModel->currentIndex();

        QVariant var = this->mModel->data(index, ImagesModel::ImageRole);
        if (var.isValid())
        {
            QImage image = var.value<QImage>();
            this->mEditor->setImage(&image);

            this->updateStatus();
        }
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_documentChanged()
{
    this->updateSelectedImage();
    emit this->documentChanged();

    if (this->mDocument->dataContainer()->count() != this->mLastImagesCount)
    {
        this->mon_documentChangedSignificantly();
        this->mLastImagesCount = this->mDocument->dataContainer()->count();
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_documentChangedSignificantly()
{
    this->mModel->callReset();
    this->updateTableFont();
    this->updateStatus();
    emit this->documentChanged();

    if (this->mDocument->dataContainer()->count() > 0)
    {
        QString key = this->currentKey();
        const QImage *image = this->mDocument->dataContainer()->image(key);
        this->mEditor->setImage(image);
    }
    else
    {
        QImage empty = QImage();
        this->mEditor->setImage(&empty);
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_editor_imageChanged()
{
    const QImage *image = this->mEditor->image();
    this->mDocument->dataContainer()->setImage(this->currentKey(), image);
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_editor_mouseMove(QPoint point)
{
    if (point.x() >= 0 && point.y() >= 0)
    {
        this->mStatusData->setData(StatusData::MouseCoordinates, QVariant(point));
    }
    else
    {
        this->mStatusData->removeData(StatusData::MouseCoordinates);
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_editor_scaleChanged(int scale)
{
    this->mStatusData->setData(StatusData::Scale, QVariant(scale));
}
//-----------------------------------------------------------------------------
void EditorTabFont::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid())
    {
        QVariant var = this->mModel->data(current, ImagesModel::KeyRole);
        QString key = var.toString();

        const QImage *image = this->mDocument->dataContainer()->image(key);
        this->mEditor->setImage(image);

        // update selection for currentKey() method, called by updateStatus()
        QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
        selectionModel->setCurrentIndex(current, QItemSelectionModel::SelectCurrent);

        this->updateStatus();
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::updateTableFont()
{
    this->resizeToContents();
}
//-----------------------------------------------------------------------------
void EditorTabFont::resizeToContents()
{
    this->ui->tableViewCharacters->resizeColumnsToContents();
    this->ui->tableViewCharacters->resizeRowsToContents();
}
//-----------------------------------------------------------------------------
/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="font" name="Font">
    <family>Ubuntu</family>
    <size>12</size>
    <bold>true</bold>
    <italic>false</italic>
    <widthType>proportional</widthType>
    <antialiasing>false</antialiasing>
    <string> !"#$%&amp;'()*+,-./0123456789:;&lt;=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВ</string>
    <converted>/tmp/font.c</converted>
    <chars>
        <char character=" " code="0020">
            <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAQAAAATCAIAAAA4QDsKAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAFUlEQVQImWP8//8/AwwwMSCBYc0BAO+LAyPoIK0eAAAAAElFTkSuQmCC</picture>
        </char>
    </chars>
</data>
 */
