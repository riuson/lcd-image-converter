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

#include "datacontainer.h"
#include "parser.h"
#include "tags.h"
#include "statusdata.h"
#include "imagedocument.h"
#include "editor.h"
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mDocument = new ImageDocument(this);

    this->mEditorObject = new ImageEditor::Editor(this);
    this->mEditorWidget = this->mEditorObject->widget();
    layout->addWidget(this->mEditorWidget);

    this->connect(this->mDocument, SIGNAL(documentChanged()), SLOT(mon_documentChanged()));
    this->connect(this->mEditorObject, SIGNAL(imageChanged(const QImage*)), SLOT(mon_editor_imageChanged(const QImage*)));
    this->connect(this->mEditorObject, SIGNAL(mouseMoved(const QPoint*)), SLOT(mon_editor_mouseMove(const QPoint*)));
    this->connect(this->mEditorObject, SIGNAL(scaleChanged(int)), SLOT(mon_editor_scaleChanged(int)));

    this->initStatusData();

    this->updateSelectedImage();
}
//-----------------------------------------------------------------------------
EditorTabImage::~EditorTabImage()
{
    this->mEditorWidget->setParent(NULL);
    delete ui;
    delete this->mEditorObject;
}
//-----------------------------------------------------------------------------
IDocument *EditorTabImage::document() const
{
    return qobject_cast<IDocument *>(this->mDocument);
}
//-----------------------------------------------------------------------------
QStringList EditorTabImage::selectedKeys() const
{
    return this->mDocument->dataContainer()->keys();
}
//-----------------------------------------------------------------------------
StatusData *EditorTabImage::statusData() const
{
    return this->mStatusData;
}
//-----------------------------------------------------------------------------
IEditor::EditorType EditorTabImage::type() const
{
    return EditorImage;
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
void EditorTabImage::updateStatus()
{
    QStringList keys = this->mDocument->dataContainer()->keys();
    if (keys.length() > 0)
    {
        const QImage *currentImage = this->mDocument->dataContainer()->image(keys.at(0));
        this->mStatusData->setData(StatusData::ImageSize, QVariant(currentImage->size()));
    }
    this->mStatusData->setData(StatusData::Scale, QVariant(this->mEditorObject->scale()));
}
//-----------------------------------------------------------------------------
void EditorTabImage::updateSelectedImage()
{
    QStringList keys = this->mDocument->dataContainer()->keys();
    if (keys.length() > 0)
    {
        const QImage *image = this->mDocument->dataContainer()->image(keys.at(0));
        this->mEditorObject->setImage(image);
    }

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_documentChanged()
{
    this->updateSelectedImage();
    emit this->documentChanged();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_imageChanged(const QImage* value)
{
    QStringList keys = this->mDocument->dataContainer()->keys();
    this->mDocument->dataContainer()->setImage(keys.at(0), value);
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_mouseMove(const QPoint* point)
{
    if (point->x() >= 0 && point->y() >= 0)
    {
        this->mStatusData->setData(StatusData::MouseCoordinates, QVariant(*point));
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
