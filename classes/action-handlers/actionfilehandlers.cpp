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
#include <QTextStream>
#include <QInputDialog>
#include <QLineEdit>
#include "parser.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "datacontainer.h"
#include "idocument.h"
#include "preset.h"
//-----------------------------------------------------------------------------
ActionFileHandlers::ActionFileHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::newImage_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                         tr("Enter image name"),
                                         tr("Image name:"),
                                         QLineEdit::Normal,
                                         QString("Image"),
                                         &ok);
    if (ok)
    {
        EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
        this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

        emit this->tabCreated(ed);

        name = this->mMainWindow->findAvailableName(name);
        ed->document()->setDocumentName(name);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::newFont_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                         tr("Enter font name"),
                                         tr("Font name:"),
                                         QLineEdit::Normal,
                                         QString("Font"),
                                         &ok);
    if (ok)
    {
        DialogFontSelect dialog(this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

            ed->document()->beginChanges();

            QString chars = dialog.characters();
            int size;
            QString family, style;
            bool monospaced, antialiasing;

            family = dialog.fontFamily();
            style = dialog.fontStyle();
            size = dialog.fontSize();
            monospaced = dialog.monospaced();
            antialiasing = dialog.antialiasing();

            ed->setFontCharacters(chars, family, style, size, monospaced, antialiasing);

            emit this->tabCreated(ed);

            name = this->mMainWindow->findAvailableName(name);
            ed->document()->setDocumentName(name);

            ed->document()->endChanges(true);
            ed->document()->dataContainer()->historyInit();
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::open_triggered()
{
    QFileDialog dialog(this->mMainWindow->parentWidget());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("XML Files (*.xml);;Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    dialog.setWindowTitle(tr("Open xml or image file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList filenames = dialog.selectedFiles();

        for (int i = 0; i < filenames.length(); i++)
        {
            this->openFile(filenames.at(i));
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::rename_triggered()
{
    bool ok;
    IEditor *editor = this->editor();
    if (editor != NULL)
    {
        QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                             tr("Rename"),
                                             tr("New name:"),
                                             QLineEdit::Normal,
                                             editor->document()->documentName(),
                                             &ok);
        if (ok)
        {
            editor->document()->setDocumentName(name);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::save_triggered()
{
    IEditor *editor = this->editor();
    if (editor != NULL)
    {
        if (QFile::exists(editor->document()->documentFilename()))
            editor->document()->save(editor->document()->documentFilename());
        else
            this->saveAs_triggered();
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::saveAs_triggered()
{
    IEditor *editor = this->editor();
    if (editor != NULL)
    {
        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("XML Files (*.xml)"));
        dialog.setDefaultSuffix(QString("xml"));
        dialog.setWindowTitle(tr("Save file as"));

        if (editor->document()->documentFilename().isEmpty())
        {
            dialog.selectFile(editor->document()->documentName());
        }
        else
        {
            dialog.selectFile(editor->document()->documentFilename());
        }

        if (dialog.exec() == QDialog::Accepted)
        {
            QString filename = dialog.selectedFiles().at(0);
            editor->document()->save(filename);

            emit this->rememberFilename(filename);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::close_triggered()
{
    emit closeRequest(this->mMainWindow->currentTab());
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::convert_triggered()
{
    IEditor *editor = this->mMainWindow->currentEditor();
    if (editor != NULL)
    {
        this->convertDocument(editor->document(), true);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::convertAll_triggered()
{
    QList<QWidget *> list;

    this->mMainWindow->tabsList(&list);

    for (int i = 0; i < list.count(); i++)
    {
        IEditor *editor = dynamic_cast<IEditor *> (list.at(i));
        if (editor != NULL)
        {
            this->convertDocument(editor->document(), false);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::openFile(const QString &filename)
{
    bool isImage = false;
    bool isFont = false;
    bool isImageBinary = false;

    QFileInfo info(filename);
    if (info.exists())
    {
        if (info.suffix().toLower() == "xml")
        {
            QFile file(filename);
            if (file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                QRegExp regImage("<data.+type=\"image\"", Qt::CaseInsensitive);
                QRegExp regFont("<data.+type=\"font\"", Qt::CaseInsensitive);
                while (!stream.atEnd())
                {
                    QString readedLine = stream.readLine();
                    if (readedLine.contains(regImage))
                    {
                        isImage = true;
                        break;
                    }
                    if (readedLine.contains(regFont))
                    {
                        isFont = true;
                        break;
                    }
                }
                file.close();

                emit this->rememberFilename(filename);
            }
        }
        else
        {
            QStringList imageExtensions;
            imageExtensions << "bmp" << "gif" << "jpg" << "jpeg" << "png" << "pbm" << "pgm" << "ppm" << "tiff" << "xbm" << "xpm";
            if (imageExtensions.contains(info.suffix().toLower()))
                isImageBinary = true;
        }
        if (isImage)
        {
            EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

            emit this->tabCreated(ed);
            ed->document()->load(filename);
        }
        if (isFont)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

            emit this->tabCreated(ed);
            ed->document()->load(filename);
        }
        if (isImageBinary)
        {
            QImage imageLoaded;
            if (imageLoaded.load(filename))
            {
                QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

                EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
                this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

                QString name = this->mMainWindow->findAvailableName(info.baseName());

                // assign image
                {
                    QStringList keys = ed->selectedKeys();

                    QStringListIterator iterator(keys);
                    while (iterator.hasNext())
                    {
                        QString key = iterator.next();
                        ed->document()->dataContainer()->setImage(key, &imageConverted);
                    }
                }

                emit this->tabCreated(ed);
                ed->document()->setDocumentName(name);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::openImage(QImage *image, const QString &documentName)
{
    EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
    this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

    QString name = this->mMainWindow->findAvailableName(documentName);

    // assign image
    {
        QStringList keys = ed->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();
            ed->document()->dataContainer()->setImage(key, image);
        }
    }

    ed->document()->setDocumentName(name);

    emit this->tabCreated(ed);
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::convertDocument(IDocument *document, bool request)
{
    // converter output file name
    QString outputFileName = document->outputFilename();

    // if file name not specified, show dialog
    bool filenameNotSpecified = outputFileName.isEmpty();

    // show dialog
    if (request || filenameNotSpecified)
    {
        QFileDialog dialog(qobject_cast<QWidget *>(this->parent()));
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(outputFileName);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("C Files (*.c);;All Files (*.*)"));
        dialog.setDefaultSuffix(QString("c"));
        dialog.setWindowTitle(tr("Save result file as"));

        if (filenameNotSpecified)
        {
            dialog.selectFile(document->documentName());
        }
        else
        {
            dialog.selectFile(outputFileName);
        }

        if (dialog.exec() == QDialog::Accepted)
        {
            outputFileName = dialog.selectedFiles().at(0);
        }
        else
        {
            outputFileName = "";
        }
    }

    // if file name specified, save result
    if (!outputFileName.isEmpty())
    {
        QFile file(outputFileName);
        if (file.open(QFile::WriteOnly))
        {
            Preset preset;
            preset.load(Preset::selectedName());

            QString result = document->convert(&preset);

            file.write(result.toUtf8());
            file.close();

            if (document->outputFilename() != outputFileName)
            {
                document->setOutputFilename(outputFileName);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::documentChanged()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    emit this->tabChanged(w);
}
//-----------------------------------------------------------------------------
