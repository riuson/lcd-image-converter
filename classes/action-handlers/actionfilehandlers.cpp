/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
#include <QSettings>
#include <QTextStream>
#include <QInputDialog>
#include <QLineEdit>
#include "parser.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "idatacontainer.h"
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
        this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

        name = this->mMainWindow->findAvailableName(name);
        ed->setDocumentName(name);
        ed->setChanged(false);
        emit this->tabCreated(ed, name, ed->fileName());
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
            this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

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
            ed->setDocumentName(name);
            ed->setChanged(false);
            emit this->tabCreated(ed, name, ed->fileName());
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::open_triggered()
{
    QFileDialog dialog(this->mMainWindow->parentWidget());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("XML Files (*.xml);;Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    dialog.setWindowTitle(tr("Open xml or image file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QString filename = dialog.selectedFiles().at(0);

        this->openFile(filename);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::rename_triggered()
{
    bool ok;
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        QString name = QInputDialog::getText(this->mMainWindow->parentWidget(),
                                             tr("Rename"),
                                             tr("New name:"),
                                             QLineEdit::Normal,
                                             doc->documentName(),
                                             &ok);
        if (ok)
        {
            doc->setDocumentName(name);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::save_triggered()
{
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        if (QFile::exists(doc->fileName()))
            doc->save(doc->fileName());
        else
            this->saveAs_triggered();
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::saveAs_triggered()
{
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setFilter(tr("XML Files (*.xml)"));
        dialog.setDefaultSuffix(QString("xml"));
        dialog.setWindowTitle(tr("Save file as"));
        if (dialog.exec() == QDialog::Accepted)
        {
            QString filename = dialog.selectedFiles().at(0);
            doc->save(filename);

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
    QSettings sett;
    sett.beginGroup("setup");

    sett.endGroup();

    QWidget *w = this->mMainWindow->currentTab();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        QMap<QString, QString> tags;
        if (!doc->fileName().isEmpty())
            tags["fileName"] = doc->fileName();
        else
            tags["fileName"] = "unknown";
        QString docName = doc->documentName();
        tags["documentName"] = docName;
        docName = docName.remove(QRegExp("\\W", Qt::CaseInsensitive));
        tags["documentName_ws"] = docName;

        Parser::TemplateType templateType = Parser::TypeImage;

        if (EditorTabImage *eti = qobject_cast<EditorTabImage *>(w))
        {
            Q_UNUSED(eti);
            tags["dataType"] = "image";
            templateType = Parser::TypeImage;
        }
        if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(w))
        {
            QString chars, fontFamily, style;
            int size;
            bool monospaced, antialiasing;
            etf->fontCharacters(&chars, &fontFamily, &style, &size, &monospaced, &antialiasing);

            tags["dataType"] = "font";
            tags["fontFamily"] = fontFamily;
            tags["fontSize"] = QString("%1").arg(size);
            tags["fontStyle"] = style;
            tags["string"] = chars;
            tags["fontAntialiasing"] = antialiasing ? "yes" : "no";
            tags["fontWidthType"] = monospaced ? "monospaced" : "proportional";

            templateType = Parser::TypeFont;
        }
        Parser parser(this, templateType);
        QString result = parser.convert(doc, tags);

        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setFilter(tr("C Files (*.c);;All Files (*.*)"));
        dialog.setDefaultSuffix(QString("c"));
        dialog.setWindowTitle(tr("Save result file as"));
        if (dialog.exec() == QDialog::Accepted)
        {
            QString filename = dialog.selectedFiles().at(0);

            QFile file(filename);
            if (file.open(QFile::WriteOnly))
            {
                file.write(result.toUtf8());
                file.close();
            }
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
                while (!stream.atEnd())
                {
                    QString readedLine = stream.readLine();
                    if (readedLine.contains("<data type=\"image\""))
                    {
                        isImage = true;
                        break;
                    }
                    if (readedLine.contains("<data type=\"font\""))
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
            this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

            emit this->tabCreated(ed, "", filename);
            ed->load(filename);
        }
        if (isFont)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

            emit this->tabCreated(ed, "", filename);
            ed->load(filename);
        }
        if (isImageBinary)
        {
            QImage image;
            if (image.load(filename))
            {
                EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
                this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

                QString name = this->mMainWindow->findAvailableName(info.baseName());

                QString key = ed->editor()->currentImageKey();
                ed->dataContainer()->setImage(key, &image);

                ed->setDocumentName(name);
                ed->setChanged(false);
                emit this->tabCreated(ed, name, filename);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::openImage(QImage *image, const QString &documentName)
{
    EditorTabImage *ed = new EditorTabImage(this->mMainWindow->parentWidget());
    this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

    QString name = this->mMainWindow->findAvailableName(documentName);

    QString key = ed->editor()->currentImageKey();
    ed->dataContainer()->setImage(key, image);

    ed->setDocumentName(name);
    ed->setChanged(true);

    emit this->tabCreated(ed, name, "");

    emit this->tabChanged(ed, "* " + name, "");
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::documentChanged(bool changed, const QString &documentName, const QString &filename)
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        if (changed)
            emit this->tabChanged(w, "* " + documentName, filename);
        else
            emit this->tabChanged(w, documentName, filename);
    }
}
//-----------------------------------------------------------------------------
