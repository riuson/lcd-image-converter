/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QSettings>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include "limits"

#include "editortabimage.h"
#include "editortabfont.h"
#include "starttab.h"
#include "bitmapcontainer.h"
#include "dialogsavechanges.h"
#include "widgetbitmapeditor.h"
#include "bitmaphelper.h"
#include "dialogresize.h"
#include "dialogconvert.h"
#include "converter.h"
#include "dialogsetuptemplates.h"
#include "dialogfontselect.h"
#include "dialogabout.h"
#include "recentlist.h"
#include "actionimagehandlers.h"
//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QIcon icon;
    icon.addFile(":/images/icon64", QSize(64, 64));
    this->setWindowIcon(icon);

    this->mEditor = NULL;

    this->updateMenuState();

    this->mTrans = new QTranslator;
    qApp->installTranslator(this->mTrans);

    QDir dir(":/translations");
    QStringList translations = dir.entryList(QDir::Files, QDir::Name);
    QStringListIterator it(translations);
    it.toFront();
    while (it.hasNext())
    {
        QString name = it.next();
        QAction *action = this->ui->menuLanguage->addAction(name, this, SLOT(actionLanguage_triggered()));
        action->setCheckable(true);
        action->setData(QVariant(name));
        QTranslator trans;
        trans.load(":/translations/" + name);
        QString a = trans.translate("MainWindow", "langName");
        action->setText(a);
    }
    this->connect(this->ui->actionLanguageDefault, SIGNAL(triggered()), SLOT(actionLanguage_triggered()));

    QSettings sett;
    sett.beginGroup("language");
    QString selectedLocale = sett.value("selected", QVariant("")).toString();
    this->selectLocale(selectedLocale);
    sett.endGroup();

    // create recent list
    this->mRecentList = new RecentList(this);
    this->connect(this->mRecentList, SIGNAL(listChanged()), SLOT(updateRecentList()));
    this->updateRecentList();

    this->mImageHandlers = new ActionImageHandlers(this);
    this->mImageHandlers->connect(this->ui->actionImageFlip_Horizontal, SIGNAL(triggered()), SLOT(on_actionImageFlip_Horizontal_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageFlip_Vertical, SIGNAL(triggered()), SLOT(on_actionImageFlip_Vertical_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_90_Clockwise, SIGNAL(triggered()), SLOT(on_actionImageRotate_90_Clockwise_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_180, SIGNAL(triggered()), SLOT(on_actionImageRotate_180_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_90_Counter_Clockwise, SIGNAL(triggered()), SLOT(on_actionImageRotate_90_Counter_Clockwise_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageInverse, SIGNAL(triggered()), SLOT(on_actionImageInverse_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageResize, SIGNAL(triggered()), SLOT(on_actionImageResize_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageImport, SIGNAL(triggered()), SLOT(on_actionImageImport_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageExport, SIGNAL(triggered()), SLOT(on_actionImageExport_triggered()));

    this->checkStartPageVisible();
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete this->mRecentList;
    delete this->mImageHandlers;
    delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
QString MainWindow::findAvailableName(const QString &prefix)
{
    QString result = prefix;
    QList<QString> names;
    for (int i = 0; i < this->ui->tabWidget->count(); i++)
    {
        QWidget *w = this->ui->tabWidget->widget(i);
        IDocument *doc = dynamic_cast<IDocument *> (w);
        if (doc != NULL)
            names.append(doc->documentName());
    }
    int i = 1;
    if (names.contains(prefix))
    {
        while (true)
        {
            QString name = QString("%1 %2").arg(prefix).arg(i++);
            if (!names.contains(name))
            {
                result = name;
                break;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
void MainWindow::updateMenuState()
{
    int index = this->ui->tabWidget->currentIndex();
    QWidget *w = this->ui->tabWidget->widget(index);
    bool editorSelected = false;
    if (EditorTabImage *eti = qobject_cast<EditorTabImage *>(w))
    {
        this->ui->menuFont->setEnabled(false);
        this->mEditor = eti->editor();
        editorSelected = true;
    }
    if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(w))
    {
        this->ui->menuFont->setEnabled(true);
        this->mEditor = etf->editor();
        editorSelected = true;
    }
    else
    {
        this->ui->menuFont->setEnabled(false);
    }
    this->ui->menuImage->setEnabled(editorSelected);

    this->ui->actionRename->setEnabled(editorSelected);
    this->ui->actionSave->setEnabled(editorSelected);
    this->ui->actionSave_As->setEnabled(editorSelected);
    this->ui->actionClose->setEnabled(editorSelected);
    this->ui->actionConvert->setEnabled(editorSelected);

    if (!editorSelected)
        this->mEditor = NULL;
}
//-----------------------------------------------------------------------------
void MainWindow::selectLocale(const QString &localeName)
{
    QSettings sett;
    sett.beginGroup("language");

    QFile file(":/translations/" + localeName);
    if (file.exists())
    {
        this->mTrans->load(":/translations/" + localeName);
        qApp->installTranslator(this->mTrans);
        sett.setValue("selected", QVariant(localeName));
    }
    else
    {
        qApp->removeTranslator(this->mTrans);
        sett.setValue("selected", QVariant(""));
    }

    sett.endGroup();

    QList<QAction *> actions =this->ui->menuLanguage->actions();
    QMutableListIterator<QAction *> it(actions);
    it.toFront();
    while (it.hasNext())
    {
        QAction *a = it.next();
        QString b = a->data().toString();
        a->setChecked(b == localeName);
    }
}
//-----------------------------------------------------------------------------
int MainWindow::appendTab(QWidget *newTab, const QString &name)
{
    int index = this->ui->tabWidget->addTab(newTab, name);
    this->ui->tabWidget->setCurrentIndex(index);
    this->checkStartPageVisible();
    return index;
}
//-----------------------------------------------------------------------------
void MainWindow::checkStartPageVisible()
{
    // show startpage if no other tabs
    int othersTabs = 0;
    int startPageIndex = -1;
    for (int i = 0; i < this->ui->tabWidget->count(); i++)
    {
        QWidget *w = this->ui->tabWidget->widget(i);
        StartTab *tab = dynamic_cast<StartTab *> (w);
        if (tab == NULL)
            othersTabs++;
        else
            startPageIndex = i;
    }

    if (othersTabs == 0)
    {
        if (startPageIndex < 0)
        {
            StartTab *tab = new StartTab(this->ui->tabWidget);
            tab->setParent(this->ui->tabWidget);
            tab->setRecentFiles(this->mRecentList->files());
            this->connect(tab, SIGNAL(openRecent(QString)), SLOT(openFile(QString)));
            this->connect(tab, SIGNAL(createNewImage()), SLOT(on_actionNew_Image_triggered()));
            this->connect(tab, SIGNAL(createNewFont()), SLOT(on_actionNew_Font_triggered()));

            this->ui->tabWidget->addTab(tab, tab->tabName());
            this->ui->tabWidget->setTabsClosable(false);
        }
    }
    else
    {
        if (startPageIndex >= 0)
        {
            StartTab *tab = dynamic_cast<StartTab *> (this->ui->tabWidget->widget(startPageIndex));
            this->ui->tabWidget->removeTab(startPageIndex);
            this->ui->tabWidget->setTabsClosable(true);
            delete tab;
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *w = this->ui->tabWidget->widget(index);
    IDocument *doc = dynamic_cast<IDocument *> (w);
    bool cancel = false;
    if (doc != NULL && doc->changed())
    {
        DialogSaveChanges dialog(this);
        dialog.exec();
        switch (dialog.answer())
        {
        case DialogSaveChanges::Save:
            {
                if (!doc->save(doc->fileName()))
                    cancel = true;
            }
            break;
        case DialogSaveChanges::SaveAs:
            {
                QFileDialog dialog(this);
                dialog.setAcceptMode(QFileDialog::AcceptSave);
                dialog.setFileMode(QFileDialog::AnyFile);
                dialog.setFilter(tr("XML Files (*.xml)"));
                dialog.setWindowTitle(tr("Save file as"));
                if (dialog.exec() == QDialog::Accepted)
                {
                    QString filename = dialog.selectedFiles().at(0);
                    if (!doc->save(filename))
                        cancel = true;
                }
                else
                    cancel = true;
            }
            break;
        case DialogSaveChanges::DontSave:
            break;
        case DialogSaveChanges::Cancel:
            cancel = true;
            break;
        }
    }
    if (!cancel)
    {
        if (this->ui->tabWidget->currentIndex() == index)
        {
            this->mEditor = NULL;
        }
        this->ui->tabWidget->removeTab(index);
        this->checkStartPageVisible();
        delete w;
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    this->updateMenuState();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionNew_Image_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Enter image name"),
                                         tr("Image name:"),
                                         QLineEdit::Normal,
                                         tr("Image", "new image name"),
                                         &ok);
    if (ok)
    {
        EditorTabImage *ed = new EditorTabImage(this);
        this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

        name = this->findAvailableName(name);
        ed->setDocumentName(name);
        ed->setChanged(false);
        this->appendTab(ed, name);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionNew_Font_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Enter font name"),
                                         tr("Font name:"),
                                         QLineEdit::Normal,
                                         tr("Font", "new font name"),
                                         &ok);
    if (ok)
    {
        DialogFontSelect dialog(this);
        //DialogCharacters dialog(this);
        if (dialog.exec() == QDialog::Accepted)
        {
            EditorTabFont *ed = new EditorTabFont(this);
            this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

            QString chars = dialog.characters();
            int size;
            QString family, style;
            bool monospaced, antialiasing;
            //dialog.selectedFont(&family, &style, &size, &monospaced, &antialiasing);
            family = dialog.fontFamily();
            style = dialog.fontStyle();
            size = dialog.fontSize();
            monospaced = dialog.monospaced();
            antialiasing = dialog.antialiasing();

            ed->setFontCharacters(chars, family, style, size, monospaced, antialiasing);

            name = this->findAvailableName(name);
            ed->setDocumentName(name);
            ed->setChanged(false);
            this->appendTab(ed, name);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
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
void MainWindow::on_actionRename_triggered()
{
    bool ok;
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        QString name = QInputDialog::getText(this,
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
void MainWindow::on_actionSave_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        if (QFile::exists(doc->fileName()))
            doc->save(doc->fileName());
        else
            this->on_actionSave_As_triggered();
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_As_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setFilter(tr("XML Files (*.xml)"));
        dialog.setDefaultSuffix(QString("xml"));
        dialog.setWindowTitle(tr("Save file as"));
        if (dialog.exec() == QDialog::Accepted)
        {
            QString filename = dialog.selectedFiles().at(0);
            doc->save(filename);

            this->mRecentList->add(filename);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionClose_triggered()
{
    this->on_tabWidget_tabCloseRequested(this->ui->tabWidget->currentIndex());
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionConvert_triggered()
{
    QSettings sett;
    sett.beginGroup("setup");

    QString templateImageFileName = sett.value("templateImage", ":/templates/image_convert").toString();
    QString templateFontFileName = sett.value("templateFont", ":/templates/font_convert").toString();

    sett.endGroup();

    QWidget *w = this->ui->tabWidget->currentWidget();
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

        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setFilter(tr("C Files (*.c);;All Files (*.*)"));
        dialog.setDefaultSuffix(QString("c"));
        dialog.setWindowTitle(tr("Save result file as"));
        if (dialog.exec() == QDialog::Accepted)
        {
            QString filename = dialog.selectedFiles().at(0);
            doc->save(filename);

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
void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionFontChange_triggered()
{
    int index = this->ui->tabWidget->currentIndex();
    QWidget *w = this->ui->tabWidget->widget(index);
    if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(w))
    {
        QString chars, fontFamily, style;
        int size;
        bool monospaced, antialiasing;
        etf->fontCharacters(&chars, &fontFamily, &style, &size, &monospaced, &antialiasing);

        DialogFontSelect dialog(this);
        dialog.setCharacters(chars);
        //dialog.selectFont(fontFamily, style, size, monospaced, antialiasing);
        dialog.setFontFamily(fontFamily);
        dialog.setFontStyle(style);
        dialog.setFontSize(size);
        dialog.setMonospaced(monospaced);
        dialog.setAntialising(antialiasing);

        if (dialog.exec() == QDialog::Accepted)
        {
            QString chars = dialog.characters();
            int size;
            QString family, style;
            bool monospaced, antialiasing;

            //dialog.selectedFont(&family, &style, &size, &monospaced, &antialiasing);
            family = dialog.fontFamily();
            style = dialog.fontStyle();
            size = dialog.fontSize();
            monospaced = dialog.monospaced();
            antialiasing = dialog.antialiasing();

            etf->setFontCharacters(chars, family, style, size, monospaced, antialiasing);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionFontInverse_triggered()
{
    if (this->mEditor != NULL)
    {
        QStringList keys = this->mEditor->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            QImage *original = this->mEditor->dataContainer()->image(key);
            QImage result(*original);
            result.invertPixels();
            this->mEditor->dataContainer()->setImage(key, &result);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionFontResize_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        DialogResize dialog(original->width(), original->height(), 0, 0, true, true, false, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);

            QStringList keys = this->mEditor->dataContainer()->keys();
            QListIterator<QString> it(keys);
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                original = this->mEditor->dataContainer()->image(key);

                QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, changeWidth, changeHeight, this->mEditor->color2());

                this->mEditor->dataContainer()->setImage(key, &result);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionFontMinimizeHeight_triggered()
{
    if (this->mEditor != NULL)
    {
        int left = std::numeric_limits<int>::max();
        int top = std::numeric_limits<int>::max();
        int right = 0;
        int bottom = 0;
        int l, t, r, b;
        int width = 0;
        int height = 0;

        // find limits
        QStringList keys = this->mEditor->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            QImage *original = this->mEditor->dataContainer()->image(key);

            BitmapHelper::findEmptyArea(original, &l, &t, &r, &b);

            left = qMin(left, l);
            top = qMin(top, t);
            right = qMax(right, r);
            bottom = qMax(bottom, b);

            width = qMax(width, original->width());
            height = qMax(height, original->height());
        }

        DialogResize dialog(width, bottom + 1 - top, 0, -top, false, false, true, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);

            QStringList keys = this->mEditor->dataContainer()->keys();
            QListIterator<QString> it(keys);
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                QImage *original = this->mEditor->dataContainer()->image(key);

                QImage result = BitmapHelper::resize(original, original->width(), height, width, offsetY, center, changeWidth, changeHeight, this->mEditor->color2());

                this->mEditor->dataContainer()->setImage(key, &result);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSetupConversion_triggered()
{
    IDataContainer *data = NULL;
    if (this->mEditor != NULL)
        data = this->mEditor->dataContainer();
    DialogConvert dialog(data, this);
    dialog.exec();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSetupTemplates_triggered()
{
    DialogSetupTemplates dialog(this);
    dialog.exec();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered()
{
    DialogAbout dialog(this);
    dialog.exec();
}
//-----------------------------------------------------------------------------
void MainWindow::actionLanguage_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->data().toString();
    this->selectLocale(name);
}
//-----------------------------------------------------------------------------
void MainWindow::mon_editor_dataChanged()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    int index = this->ui->tabWidget->indexOf(w);
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc != NULL)
    {
        if (doc->changed())
            this->ui->tabWidget->setTabText(index, "* " + doc->documentName());
        else
            this->ui->tabWidget->setTabText(index, doc->documentName());
        this->ui->tabWidget->setTabToolTip(index, doc->fileName());
    }
}
//-----------------------------------------------------------------------------
void MainWindow::updateRecentList()
{
    QList<QAction *> actions = this->ui->menuRecent->actions();
    QAction *action;
    foreach(action, actions)
    {
        this->ui->menuRecent->removeAction(action);
    }
    for (int i = actions.count() - 1; i >= 0; i--)
    {
        action = actions.at(i);
        delete action;
    }

    if (this->mRecentList->files()->count() > 0)
    {
        this->ui->menuRecent->setEnabled(true);
        for (int i = 0; i < this->mRecentList->files()->count(); i++)
        {
            QString filename = this->mRecentList->files()->at(i);
            QString strippedName = QFileInfo(filename).fileName();
            QString text = tr("%1 %2").arg(i + 1).arg(strippedName);

            action = new QAction(text, this);
            action->setData(filename);
            this->connect(action, SIGNAL(triggered()), SLOT(openRecentFile()));
            //action->setVisible(true);
            this->ui->menuRecent->addAction(action);
        }
    }
    else
        this->ui->menuRecent->setEnabled(false);
}
//-----------------------------------------------------------------------------
void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        this->openFile(action->data().toString());
}
//-----------------------------------------------------------------------------
void MainWindow::openFile(const QString &filename)
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

                this->mRecentList->add(filename);
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
            EditorTabImage *ed = new EditorTabImage(this);
            this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

            int index = this->appendTab(ed, "");
            ed->load(filename);
            this->ui->tabWidget->setTabText(index, ed->documentName());
        }
        if (isFont)
        {
            EditorTabFont *ed = new EditorTabFont(this);
            this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

            int index = this->appendTab(ed, "");
            ed->load(filename);
            this->ui->tabWidget->setTabText(index, ed->documentName());
        }
        if (isImageBinary)
        {
            QImage image;
            if (image.load(filename))
            {
                EditorTabImage *ed = new EditorTabImage(this);
                this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

                QString name = this->findAvailableName(info.baseName());

                QString key = ed->editor()->currentImageKey();
                ed->dataContainer()->setImage(key, &image);

                ed->setDocumentName(name);
                ed->setChanged(false);
                this->appendTab(ed, name);
            }
        }
    }
}
//-----------------------------------------------------------------------------
IDocument *MainWindow::currentDocument()
{
    IDocument *result = NULL;

    int index = this->ui->tabWidget->currentIndex();
    if (index >= 0)
    {
        QWidget *w = this->ui->tabWidget->widget(index);
        result = qobject_cast<IDocument *>(w);
    }

    return result;
}
//-----------------------------------------------------------------------------
QWidget *MainWindow::parentWidget()
{
    return this;
}
//-----------------------------------------------------------------------------
