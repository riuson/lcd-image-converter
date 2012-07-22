#include "actionfilehandlers.h"

#include "editortabimage.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include <QFileDialog>
#include <QSettings>
#include <QTextStream>
#include <QInputDialog>
#include <QLineEdit>
#include "converter.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
ActionFileHandlers::ActionFileHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionNew_Image_triggered()
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
        this->mMainWindow->appendTab(ed, name);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionNew_Font_triggered()
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
            this->mMainWindow->appendTab(ed, name);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionOpen_triggered()
{
    QFileDialog dialog(this->mMainWindow->parentWidget());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("XML Files (*.xml);;Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    dialog.setWindowTitle(tr("Open file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QString filename = dialog.selectedFiles().at(0);

        this->openFile(filename);
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionRename_triggered()
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
void ActionFileHandlers::on_actionSave_triggered()
{
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        if (QFile::exists(doc->fileName()))
            doc->save(doc->fileName());
        else
            this->on_actionSave_As_triggered();
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionSave_As_triggered()
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

            emit this->newFileOpened(filename);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionClose_triggered()
{
    emit closeRequest(this->mMainWindow->currentTab());
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::on_actionConvert_triggered()
{
    QSettings sett;
    sett.beginGroup("setup");

    QString templateImageFileName = sett.value("templateImage", ":/templates/image_convert").toString();
    QString templateFontFileName = sett.value("templateFont", ":/templates/font_convert").toString();

    sett.endGroup();

    QWidget *w = this->mMainWindow->currentTab();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        QMap<QString, QString> tags;
        tags["fileName"] = doc->fileName();
        QString docName = doc->documentName();
        tags["documentName"] = docName;
        docName = docName.remove(QRegExp("\\W", Qt::CaseInsensitive));
        tags["documentName_ws"] = docName;

        QString templateFileName;

        if (EditorTabImage *eti = qobject_cast<EditorTabImage *>(w))
        {
            Q_UNUSED(eti);
            tags["dataType"] = "image";
            templateFileName = templateImageFileName;
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
            tags["fontAntialiasing"] = antialiasing ? "true" : "false";
            tags["fontWidthType"] = monospaced ? "monospaced" : "proportional";

            templateFileName = templateFontFileName;
        }
        Converter conv(this);
        QString result = conv.convert(doc, templateFileName, tags);

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
void ActionFileHandlers::on_actionQuit_triggered()
{
    emit this->exitRequest();
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

                emit this->newFileOpened(filename);
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

            this->mMainWindow->appendTab(ed, "");
            ed->load(filename);
            this->mMainWindow->setTabText(ed, ed->documentName(), filename);
        }
        if (isFont)
        {
            EditorTabFont *ed = new EditorTabFont(this->mMainWindow->parentWidget());
            this->connect(ed, SIGNAL(documentChanged(bool,QString,QString)), SLOT(documentChanged(bool,QString,QString)));

            this->mMainWindow->appendTab(ed, "");
            ed->load(filename);
            this->mMainWindow->setTabText(ed, ed->documentName(), filename);
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
                this->mMainWindow->appendTab(ed, name);
                this->mMainWindow->setTabText(ed, name, filename);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFileHandlers::documentChanged(bool changed, const QString &documentName, const QString &filename)
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        if (doc->changed())
            this->mMainWindow->setTabText(w, "* " + doc->documentName(), doc->fileName());
        else
            this->mMainWindow->setTabText(w, doc->documentName(), doc->fileName());
    }
}
//-----------------------------------------------------------------------------
