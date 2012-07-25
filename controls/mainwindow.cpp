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

#include "editortabimage.h"
#include "editortabfont.h"
#include "starttab.h"
#include "bitmapcontainer.h"
#include "dialogsavechanges.h"
#include "widgetbitmapeditor.h"
#include "revisionlabel.h"
#include "recentlist.h"
#include "actionfilehandlers.h"
#include "actionimagehandlers.h"
#include "actionfonthandlers.h"
#include "actionsetuphandlers.h"
#include "actionhelphandlers.h"
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

    this->statusBar()->addWidget(new RevisionLabel);

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

    this->createHandlers();

    this->checkStartPageVisible();
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete this->mRecentList;
    delete this->mImageHandlers;
    delete this->mFontHandlers;
    delete this->mHelpHandlers;
    delete this->mFileHandlers;
    delete this->mSetupHandlers;
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
            this->mFileHandlers->connect(tab, SIGNAL(openRecent(QString)), SLOT(openFile(QString)));
            this->mFileHandlers->connect(tab, SIGNAL(createNewImage()), SLOT(newImage_triggered()));
            this->mFileHandlers->connect(tab, SIGNAL(createNewFont()), SLOT(newFont_triggered()));

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
void MainWindow::createHandlers()
{
    this->mFileHandlers = new ActionFileHandlers(this);
    this->mFileHandlers->connect(this->ui->actionNew_Image, SIGNAL(triggered()), SLOT(newImage_triggered()));
    this->mFileHandlers->connect(this->ui->actionNew_Font, SIGNAL(triggered()), SLOT(newFont_triggered()));
    this->mFileHandlers->connect(this->ui->actionOpen, SIGNAL(triggered()), SLOT(open_triggered()));
    this->mFileHandlers->connect(this->ui->actionRename, SIGNAL(triggered()), SLOT(rename_triggered()));
    this->mFileHandlers->connect(this->ui->actionSave, SIGNAL(triggered()), SLOT(save_triggered()));
    this->mFileHandlers->connect(this->ui->actionSave_As, SIGNAL(triggered()), SLOT(saveAs_triggered()));
    this->mFileHandlers->connect(this->ui->actionClose, SIGNAL(triggered()), SLOT(close_triggered()));
    this->mFileHandlers->connect(this->ui->actionConvert, SIGNAL(triggered()), SLOT(convert_triggered()));
    this->connect(this->ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
    this->connect(this->mFileHandlers, SIGNAL(rememberFilename(QString)), SLOT(rememberFilename(QString)));
    this->connect(this->mFileHandlers, SIGNAL(closeRequest(QWidget*)), SLOT(closeRequest(QWidget*)));
    this->connect(this->mFileHandlers, SIGNAL(tabChanged(QWidget*,QString,QString)), SLOT(tabChanged(QWidget*,QString,QString)));
    this->connect(this->mFileHandlers, SIGNAL(tabCreated(QWidget*,QString,QString)), SLOT(tabCreated(QWidget*,QString,QString)));

    this->mImageHandlers = new ActionImageHandlers(this);
    this->mImageHandlers->connect(this->ui->actionImageFlip_Horizontal, SIGNAL(triggered()), SLOT(flipHorizontal_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageFlip_Vertical, SIGNAL(triggered()), SLOT(flipVertical_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_90_Clockwise, SIGNAL(triggered()), SLOT(rotate_90_Clockwise_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_180, SIGNAL(triggered()), SLOT(rotate_180_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageRotate_90_Counter_Clockwise, SIGNAL(triggered()), SLOT(rotate_90_Counter_Clockwise_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageInverse, SIGNAL(triggered()), SLOT(inverse_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageResize, SIGNAL(triggered()), SLOT(resize_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageImport, SIGNAL(triggered()), SLOT(import_triggered()));
    this->mImageHandlers->connect(this->ui->actionImageExport, SIGNAL(triggered()), SLOT(export_triggered()));

    this->mFontHandlers = new ActionFontHandlers(this);
    this->mFontHandlers->connect(this->ui->actionFontChange, SIGNAL(triggered()), SLOT(fontChange_triggered()));
    this->mFontHandlers->connect(this->ui->actionFontInverse, SIGNAL(triggered()), SLOT(fontInverse_triggered()));
    this->mFontHandlers->connect(this->ui->actionFontResize, SIGNAL(triggered()), SLOT(fontResize_triggered()));
    this->mFontHandlers->connect(this->ui->actionFontMinimizeHeight, SIGNAL(triggered()), SLOT(fontMinimizeHeight_triggered()));
    this->mFontHandlers->connect(this->ui->actionFontPreview, SIGNAL(triggered()), SLOT(fontPreview_triggered()));

    this->mSetupHandlers = new ActionSetupHandlers(this);
    this->mSetupHandlers->connect(this->ui->actionSetupConversion, SIGNAL(triggered()), SLOT(conversion_triggered()));
    this->mSetupHandlers->connect(this->ui->actionSetupTemplates, SIGNAL(triggered()), SLOT(templates_triggered()));

    this->mHelpHandlers = new ActionHelpHandlers(this);
    this->mHelpHandlers->connect(this->ui->actionAbout, SIGNAL(triggered()), SLOT(about_triggered()));
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
void MainWindow::actionLanguage_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->data().toString();
    this->selectLocale(name);
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
        this->mFileHandlers->openFile(action->data().toString());
}
//-----------------------------------------------------------------------------
void MainWindow::rememberFilename(const QString &filename)
{
    this->mRecentList->add(filename);
}
//-----------------------------------------------------------------------------
void MainWindow::closeRequest(QWidget *tab)
{
    int index = this->ui->tabWidget->indexOf(tab);
    if (index >= 0)
        this->on_tabWidget_tabCloseRequested(index);
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
QWidget *MainWindow::currentTab()
{
    int index = this->ui->tabWidget->currentIndex();
    if (index >= 0)
    {
        QWidget *w = this->ui->tabWidget->widget(index);
        return w;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
QWidget *MainWindow::parentWidget()
{
    return this;
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
void MainWindow::tabChanged(QWidget *tab, const QString &text, const QString &tooltip)
{
    int index = this->ui->tabWidget->indexOf(tab);
    if (index >= 0)
    {
        this->ui->tabWidget->setTabText(index, text);
        this->ui->tabWidget->setTabToolTip(index, tooltip);
    }
}
//-----------------------------------------------------------------------------
int MainWindow::tabCreated(QWidget *newTab, const QString &name, const QString &tooltip)
{
    int index = this->ui->tabWidget->addTab(newTab, name);
    this->ui->tabWidget->setCurrentIndex(index);
    this->ui->tabWidget->setTabToolTip(index, tooltip);
    this->checkStartPageVisible();
    return index;
}
//-----------------------------------------------------------------------------
