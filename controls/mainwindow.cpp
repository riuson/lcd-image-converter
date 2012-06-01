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
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
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
void MainWindow::openFile(const QString &filename)
{
    bool isImage = false;
    bool isFont = false;

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
    if (isImage)
    {
        EditorTabImage *ed = new EditorTabImage(this);
        this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

        int index = this->ui->tabWidget->addTab(ed, "");
        ed->load(filename);
        this->ui->tabWidget->setTabText(index, ed->documentName());
    }
    if (isFont)
    {
        EditorTabFont *ed = new EditorTabFont(this);
        this->connect(ed, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

        int index = this->ui->tabWidget->addTab(ed, "");
        ed->load(filename);
        this->ui->tabWidget->setTabText(index, ed->documentName());
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
        this->ui->tabWidget->addTab(ed, name);
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
            this->ui->tabWidget->addTab(ed, name);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("XML Files (*.xml)"));
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
    QString name = QInputDialog::getText(this,
                                         tr("Rename"),
                                         tr("New name:"),
                                         QLineEdit::Normal,
                                         doc->documentName(),
                                         &ok);
    if (ok)
    {
        doc->setDocumentName(name);
        this->ui->tabWidget->setTabText(this->ui->tabWidget->currentIndex(), name);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (QFile::exists(doc->fileName()))
        doc->save(doc->fileName());
    else
        this->on_actionSave_As_triggered();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_As_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);

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
//-----------------------------------------------------------------------------
void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageFlip_Horizontal_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result = BitmapHelper::flipHorizontal(original);
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageFlip_Vertical_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result = BitmapHelper::flipVertical(original);
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_90_Clockwise_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate90(original);
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_180_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate180(original);
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_90_Counter_Clockwise_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate270(original);
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageInverse_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        QImage result(*original);
        result.invertPixels();
        this->mEditor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageResize_triggered()
{
    if (this->mEditor != NULL)
    {
        QString key = this->mEditor->currentImageKey();
        QImage *original = this->mEditor->dataContainer()->image(key);
        DialogResize dialog(original->width(), original->height(), 0, 0, false, true, true, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);
            QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, changeWidth, changeHeight, this->mEditor->color2());
            this->mEditor->dataContainer()->setImage(key, &result);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageImport_triggered()
{
    if (this->mEditor != NULL)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
        dialog.setWindowTitle(tr("Open image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            QImage image;
            image.load(dialog.selectedFiles().at(0));
            QString key = this->mEditor->currentImageKey();
            this->mEditor->dataContainer()->setImage(key, &image);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageExport_triggered()
{
    if (this->mEditor != NULL)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString filter = tr("Windows Bitmap (*.bmp);;" \
                            "Joint Photographic Experts Group (*.jpg *.jpeg);;" \
                            "Portable Network Graphics (*.png);;" \
                            "Portable Pixmap (*.ppm);;" \
                            "Tagged Image File Format (*.tiff);;" \
                            "X11 Bitmap (*.xbm);;" \
                            "X11 Bitmap (*.xpm)");
        dialog.setFilter(filter);
        dialog.setWindowTitle(tr("Save image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            filter = dialog.selectedFilter();
            QString ext = "png";
            if (filter.contains("bmp"))
                ext = "bmp";
            else if (filter.contains("jpg"))
                ext = "jpg";
            else if (filter.contains("png"))
                ext = "png";
            else if (filter.contains("ppm"))
                ext = "ppm";
            else if (filter.contains("tiff"))
                ext = "tiff";
            else if (filter.contains("xbm"))
                ext = "xbm";
            else if (filter.contains("xpm"))
                ext = "xpm";

            QString filename = dialog.selectedFiles().at(0);
            QFileInfo info(filename);
            QString fileExt = info.suffix().toLower();
            if (fileExt.isEmpty() || fileExt != ext)
            {
                filename += "." + ext;
            }
            QString key = this->mEditor->currentImageKey();
            this->mEditor->dataContainer()->image(key)->save(filename);
        }
    }
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
void MainWindow::mon_editor_dataChanged()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    int index = this->ui->tabWidget->indexOf(w);
    IDocument *doc = dynamic_cast<IDocument *> (w);

    if (doc->changed())
        this->ui->tabWidget->setTabText(index, "* " + doc->documentName());
    else
        this->ui->tabWidget->setTabText(index, doc->documentName());
    this->ui->tabWidget->setTabToolTip(index, doc->fileName());
}
//-----------------------------------------------------------------------------
#include <QDebug>
void MainWindow::actionLanguage_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->data().toString();
    this->selectLocale(name);
}
//-----------------------------------------------------------------------------
