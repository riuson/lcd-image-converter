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

#include "editortabimage.h"
#include "ui_editortabimage.h"
//-----------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include "widgetbitmapeditor.h"
#include "datacontainer.h"
#include "parser.h"
#include "tags.h"
#include "statusdata.h"
#include "imagedocument.h"
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mDocument = new ImageDocument(this);

    this->mEditor = new WidgetBitmapEditor(this);
    layout->addWidget(this->mEditor);

    this->connect(this->mDocument, SIGNAL(documentChanged()), SLOT(mon_documentChanged()));
    this->connect(this->mEditor, SIGNAL(imageChanged()), SLOT(mon_editor_imageChanged()));
    this->connect(this->mEditor, SIGNAL(mouseMove(QPoint)), SLOT(mon_editor_mouseMove(QPoint)));
    this->connect(this->mEditor, SIGNAL(scaleSchanged(int)), SLOT(mon_editor_scaleChanged(int)));

    this->initStatusData();

    this->updateSelectedImage();
}
//-----------------------------------------------------------------------------
EditorTabImage::~EditorTabImage()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void EditorTabImage::changeEvent(QEvent *e)
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
void EditorTabImage::initStatusData()
{
    this->mStatusData = new StatusData(this);
    this->connect(this->mStatusData, SIGNAL(changed()), SIGNAL(statusChanged()));
    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::updateSelectedImage()
{
    QStringList keys = this->mDocument->selectedKeys();
    const QImage *image = this->mDocument->dataContainer()->image(keys.at(0));
    this->mEditor->setImage(image);

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_documentChanged()
{
    this->updateSelectedImage();
    this->setChanged(true);
    emit this->documentChanged(true, this->documentName(), this->fileName());
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_imageChanged()
{
    this->beginChanges();

    QStringList keys = this->mDocument->selectedKeys();
    const QImage *image = this->mEditor->image();
    this->mDocument->dataContainer()->setImage(keys.at(0), image);

    this->endChanges();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_mouseMove(QPoint point)
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
void EditorTabImage::mon_editor_scaleChanged(int scale)
{
    this->mStatusData->setData(StatusData::Scale, QVariant(scale));
}
//-----------------------------------------------------------------------------
bool EditorTabImage::load(const QString &fileName)
{
    return this->mDocument->load(fileName);
}
//-----------------------------------------------------------------------------
bool EditorTabImage::save(const QString &fileName)
{
    return this->mDocument->save(fileName);
}
//-----------------------------------------------------------------------------
bool EditorTabImage::changed() const
{
    return this->mDocument->changed();
}
//-----------------------------------------------------------------------------
void EditorTabImage::setChanged(bool value)
{
    this->mDocument->setChanged(value);
}
//-----------------------------------------------------------------------------
QString EditorTabImage::fileName() const
{
    return this->mDocument->fileName();
}
//-----------------------------------------------------------------------------
QString EditorTabImage::documentName() const
{
    return this->mDocument->documentName();
}
//-----------------------------------------------------------------------------
void EditorTabImage::setDocumentName(const QString &value)
{
    this->mDocument->setDocumentName(value);
}
//-----------------------------------------------------------------------------
DataContainer *EditorTabImage::dataContainer()
{
    return this->mDocument->dataContainer();
}
//-----------------------------------------------------------------------------
QStringList EditorTabImage::selectedKeys() const
{
    return this->mDocument->selectedKeys();
}
//-----------------------------------------------------------------------------
void EditorTabImage::convert(bool request)
{
    this->mDocument->convert(request);
}
//-----------------------------------------------------------------------------
void EditorTabImage::updateStatus()
{
    QStringList keys = this->mDocument->selectedKeys();
    const QImage *currentImage = this->mDocument->dataContainer()->image(keys.at(0));
    this->mStatusData->setData(StatusData::ImageSize, QVariant(currentImage->size()));

    this->mStatusData->setData(StatusData::Scale, QVariant(this->mEditor->scale()));
}
//-----------------------------------------------------------------------------
StatusData *EditorTabImage::statusData() const
{
    return this->mStatusData;
}
//-----------------------------------------------------------------------------
void EditorTabImage::beginChanges()
{
    this->mDocument->beginChanges();
}
//-----------------------------------------------------------------------------
void EditorTabImage::endChanges()
{
    this->mDocument->endChanges();
}
//-----------------------------------------------------------------------------
bool EditorTabImage::canUndo()
{
    return this->mDocument->canUndo();
}
//-----------------------------------------------------------------------------
bool EditorTabImage::canRedo()
{
    return this->mDocument->canRedo();
}
//-----------------------------------------------------------------------------
void EditorTabImage::undo()
{
    this->mDocument->undo();

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::redo()
{
    this->mDocument->redo();

    this->updateStatus();
}
//-----------------------------------------------------------------------------
/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="documentName">
    <picture format="png">
        Base64 string
    </picture>
</data>

<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="Image">
    <converted>/tmp/font.c</converted>
    <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAAsTAAALEwEAmpwYAAAARklEQVQYlYWPSQrAMAwDpZAX5x/Nl6eHgLMQt8IniRGWAeWqkux+zaD5my5B2z0uzHq0XegjW1+ZdLhbB4mkB0iHjY6fYS/sJjZR2Wu+lAAAAABJRU5ErkJggg==</picture>
</data>

 */
