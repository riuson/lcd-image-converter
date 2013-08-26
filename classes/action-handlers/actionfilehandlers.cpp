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
                                         tr("Image", "new image name"),
                                         &ok);
    if (ok)
    {
        EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
        this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

        name = this->mMainWindow->findAvailableName(name);
        ed->document()->setDocumentName(name);
        emit this->tabCreated(ed, name, ed->document()->documentFilename());
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
                                         tr("Font", "new font name"),
                                         &ok);
    if (ok)
    {
        DialogFontSelect dialog(this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

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

            name = this->mMainWindow->findAvailableName(name);
            ed->document()->setDocumentName(name);
            emit this->tabCreated(ed, name, ed->document()->documentFilename());
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
            editor->document()->beginChanges();
            editor->document()->setDocumentName(name);
            editor->document()->endChanges();
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
        dialog.selectFile(editor->document()->documentFilename());
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("XML Files (*.xml)"));
        dialog.setDefaultSuffix(QString("xml"));
        dialog.setWindowTitle(tr("Save file as"));
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
        editor->document()->convert(true);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::convertAll_triggered()
{
    QList<QWidget *> list;

    this->mMainWindow->tabsList(&list);

    for (int i = 0; i < list.count(); i++)
    {
        IDocument *doc = dynamic_cast<IDocument *> (list.at(i));
        if (doc != NULL)
        {
            doc->convert(false);
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

            emit this->tabCreated(ed, "", filename);
            ed->document()->load(filename);
        }
        if (isFont)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged()), SLOT(documentChanged()));

            emit this->tabCreated(ed, "", filename);
            ed->document()->load(filename);
        }
        if (isImageBinary)
        {
            QImage image;
            if (image.load(filename))
            {
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
                        ed->document()->dataContainer()->setImage(key, &image);
                    }
                }

                ed->document()->setDocumentName(name);
                emit this->tabCreated(ed, name, filename);
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

    emit this->tabCreated(ed, name, "");

    emit this->tabChanged(ed, "* " + name, "");
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::documentChanged()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    IEditor *editor = dynamic_cast<IEditor *> (w);
    if (editor != NULL)
    {
        if (editor->document()->changed())
            emit this->tabChanged(w, "* " + editor->document()->documentName(), editor->document()->documentFilename());
        else
            emit this->tabChanged(w, editor->document()->documentName(), editor->document()->documentFilename());
    }
}
//-----------------------------------------------------------------------------
