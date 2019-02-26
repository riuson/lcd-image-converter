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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QIcon>

#include "qt-version-check.h"
#include "editortabimage.h"
#include "editortabfont.h"
#include "starttab.h"
#include "datacontainer.h"
#include "dialogsavechanges.h"
#include "widgetbitmapeditor.h"
#include "recentlist.h"
#include "statusmanager.h"
#include "languageoptions.h"
#include "actionfilehandlers.h"
#include "actionedithandlers.h"
#include "actionimagehandlers.h"
#include "actionfonthandlers.h"
#include "actionsetuphandlers.h"
#include "actionhelphandlers.h"
#include "ieditor.h"
#include "idocument.h"
#include "bitmaphelper.h"
#include "filedialogoptions.h"

namespace AppUI
{

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QIcon icon;
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 16)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 24)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 32)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 48)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 64)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 128)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 256)));
  this->setWindowIcon(icon);

  this->updateMenuState();

  this->mTrans = new QTranslator;
  qApp->installTranslator(this->mTrans);

  this->mStatusManager = new Status::StatusManager(this->ui->statusBar, this);

  QDir dir(":/translations");
  QStringList translations = dir.entryList(QDir::Files, QDir::Name);
  QStringListIterator it(translations);
  it.toFront();

  while (it.hasNext()) {
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

  QString selectedLocale = Settings::LanguageOptions::locale();
  this->selectLocale(selectedLocale);

  // create recent list
  this->mRecentList = new Settings::RecentList(this);
  this->connect(this->mRecentList, SIGNAL(listChanged()), SLOT(updateRecentList()));
  this->updateRecentList();

  this->createHandlers();

  this->connect(this->ui->menuEdit, SIGNAL(aboutToShow()), SLOT(updateMenuState()));

  this->checkStartPageVisible();
}

MainWindow::~MainWindow()
{
  delete this->mRecentList;
  delete this->mImageHandlers;
  delete this->mFontHandlers;
  delete this->mHelpHandlers;
  delete this->mEditHandlers;
  delete this->mFileHandlers;
  delete this->mSetupHandlers;
  delete ui;
}

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

void MainWindow::selectLocale(const QString &localeName)
{
  if (Settings::LanguageOptions::setLocale(localeName)) {
    this->mTrans->load(":/translations/" + localeName);
    qApp->installTranslator(this->mTrans);
  } else {
    qApp->removeTranslator(this->mTrans);
  }

  QList<QAction *> actions = this->ui->menuLanguage->actions();
  QMutableListIterator<QAction *> it(actions);
  it.toFront();

  while (it.hasNext()) {
    QAction *a = it.next();
    QString b = a->data().toString();
    a->setChecked(b == localeName);
  }
}

void MainWindow::checkStartPageVisible()
{
  // show startpage if no other tabs
  int othersTabs = 0;
  int startPageIndex = -1;

  for (int i = 0; i < this->ui->tabWidget->count(); i++) {
    QWidget *w = this->ui->tabWidget->widget(i);
    AppUI::Start::StartTab *tab = dynamic_cast<AppUI::Start::StartTab *> (w);

    if (tab == nullptr) {
      othersTabs++;
    } else {
      startPageIndex = i;
    }
  }

  if (othersTabs == 0) {
    if (startPageIndex < 0) {
      AppUI::Start::StartTab *tab = new AppUI::Start::StartTab(this->ui->tabWidget);
      tab->setParent(this->ui->tabWidget);
      tab->setRecentFiles(this->mRecentList->files());
      this->mFileHandlers->connect(tab, SIGNAL(openRecent(QString)), SLOT(openFile(QString)));
      this->mFileHandlers->connect(tab, SIGNAL(createNewImage()), SLOT(newImage_triggered()));
      this->mFileHandlers->connect(tab, SIGNAL(createNewFont()), SLOT(newFont_triggered()));

      this->ui->tabWidget->addTab(tab, tab->tabName());
      this->ui->tabWidget->setTabsClosable(false);
    }
  } else {
    if (startPageIndex >= 0) {
      AppUI::Start::StartTab *tab = dynamic_cast<AppUI::Start::StartTab *> (this->ui->tabWidget->widget(startPageIndex));
      this->ui->tabWidget->removeTab(startPageIndex);
      this->ui->tabWidget->setTabsClosable(true);
      delete tab;
    }
  }
}

void MainWindow::createHandlers()
{
  this->mFileHandlers = new MenuHandlers::ActionFileHandlers(this);
  this->mFileHandlers->connect(this->ui->actionNew_Image, SIGNAL(triggered()), SLOT(newImage_triggered()));
  this->mFileHandlers->connect(this->ui->actionNew_Font, SIGNAL(triggered()), SLOT(newFont_triggered()));
  this->mFileHandlers->connect(this->ui->actionOpen, SIGNAL(triggered()), SLOT(open_triggered()));
  this->mFileHandlers->connect(this->ui->actionRename, SIGNAL(triggered()), SLOT(rename_triggered()));
  this->mFileHandlers->connect(this->ui->actionSave, SIGNAL(triggered()), SLOT(save_triggered()));
  this->mFileHandlers->connect(this->ui->actionSave_As, SIGNAL(triggered()), SLOT(saveAs_triggered()));
  this->mFileHandlers->connect(this->ui->actionClose, SIGNAL(triggered()), SLOT(close_triggered()));
  this->mFileHandlers->connect(this->ui->actionClose_All, SIGNAL(triggered()), SLOT(closeAll_triggered()));
  this->mFileHandlers->connect(this->ui->actionConvert, SIGNAL(triggered()), SLOT(convert_triggered()));
  this->mFileHandlers->connect(this->ui->actionConvert_All, SIGNAL(triggered()), SLOT(convertAll_triggered()));
  this->connect(this->ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
  this->connect(this->mFileHandlers, SIGNAL(rememberFilename(QString)), SLOT(rememberFilename(QString)));
  this->connect(this->mFileHandlers, SIGNAL(closeRequest(QWidget *)), SLOT(closeRequest(QWidget *)));
  this->connect(this->mFileHandlers, SIGNAL(tabChanged(QWidget *)), SLOT(tabChanged(QWidget *)));
  this->connect(this->mFileHandlers, SIGNAL(tabCreated(QWidget *)), SLOT(tabCreated(QWidget *)));
  this->connect(this->mFileHandlers, SIGNAL(tabSelect(QWidget *)), SLOT(setCurrentTab(QWidget *)));

  this->mEditHandlers = new MenuHandlers::ActionEditHandlers(this);
  this->mEditHandlers->connect(this->ui->actionEditUndo, SIGNAL(triggered()), SLOT(undo_triggered()));
  this->mEditHandlers->connect(this->ui->actionEditRedo, SIGNAL(triggered()), SLOT(redo_triggered()));
  this->mEditHandlers->connect(this->ui->actionEditCopy, SIGNAL(triggered()), SLOT(copy_triggered()));
  this->mEditHandlers->connect(this->ui->actionEditPaste, SIGNAL(triggered()), SLOT(paste_triggered()));

  this->mImageHandlers = new MenuHandlers::ActionImageHandlers(this);
  this->mImageHandlers->connect(this->ui->actionImageFlip_Horizontal, SIGNAL(triggered()), SLOT(flipHorizontal_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageFlip_Vertical, SIGNAL(triggered()), SLOT(flipVertical_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageRotate_90_Clockwise, SIGNAL(triggered()), SLOT(rotate_90_Clockwise_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageRotate_180, SIGNAL(triggered()), SLOT(rotate_180_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageRotate_90_Counter_Clockwise, SIGNAL(triggered()), SLOT(rotate_90_Counter_Clockwise_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageShiftLeft, SIGNAL(triggered()), SLOT(shift_left_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageShiftRight, SIGNAL(triggered()), SLOT(shift_right_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageShiftUp, SIGNAL(triggered()), SLOT(shift_up_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageShiftDown, SIGNAL(triggered()), SLOT(shift_down_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageInverse, SIGNAL(triggered()), SLOT(inverse_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageResize, SIGNAL(triggered()), SLOT(resize_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageTo_Grayscale, SIGNAL(triggered()), SLOT(grayscale_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageImport, SIGNAL(triggered()), SLOT(import_triggered()));
  this->mImageHandlers->connect(this->ui->actionImageExport, SIGNAL(triggered()), SLOT(export_triggered()));
  this->mImageHandlers->connect(this->ui->actionEdit_in_external_tool, SIGNAL(triggered()), SLOT(edit_in_external_tool_triggered()));

  this->mFontHandlers = new MenuHandlers::ActionFontHandlers(this);
  this->mFontHandlers->connect(this->ui->actionFontChange, SIGNAL(triggered()), SLOT(fontChange_triggered()));
  this->mFontHandlers->connect(this->ui->actionFontInverse, SIGNAL(triggered()), SLOT(fontInverse_triggered()));
  this->mFontHandlers->connect(this->ui->actionFontResize, SIGNAL(triggered()), SLOT(fontResize_triggered()));
  this->mFontHandlers->connect(this->ui->actionFontPreview, SIGNAL(triggered()), SLOT(fontPreview_triggered()));
  this->mFontHandlers->connect(this->ui->actionFontToImage, SIGNAL(triggered()), SLOT(fontToImage_triggered()));

  this->mSetupHandlers = new MenuHandlers::ActionSetupHandlers(this);
  this->mSetupHandlers->connect(this->ui->actionConversionOptions, SIGNAL(triggered()), SLOT(conversion_triggered()));
  this->mSetupHandlers->connect(this->ui->actionExternal_editor, SIGNAL(triggered()), SLOT(external_editor_triggered()));

  this->mHelpHandlers = new MenuHandlers::ActionHelpHandlers(this);
  this->mHelpHandlers->connect(this->ui->actionAboutApp, SIGNAL(triggered()), SLOT(about_application_triggered()));
  this->mHelpHandlers->connect(this->ui->actionAboutQt, SIGNAL(triggered()), SLOT(about_qt_triggered()));
  this->mHelpHandlers->connect(this->ui->actionUpdates, SIGNAL(triggered()), SLOT(updates_triggered()));
  this->mHelpHandlers->connect(this->ui->actionHomePage, SIGNAL(triggered()), SLOT(homepage_triggered()));

  this->mFileHandlers->connect(this->mFontHandlers, SIGNAL(imageCreated(QImage *, QString)), SLOT(openImage(QImage *, QString)));
}

void MainWindow::tabTextUpdate(QWidget *widget)
{
  int index = this->ui->tabWidget->indexOf(widget);

  if (index >= 0) {
    IEditor *editor = qobject_cast<IEditor *>(widget);

    if (editor != nullptr) {
      QString name;

      if (editor->document()->changed()) {
        name = "* " + editor->document()->documentName();
      } else {
        name = editor->document()->documentName();
      }

      this->ui->tabWidget->setTabText(index, name);
      this->ui->tabWidget->setTabToolTip(index, editor->document()->documentFilename());
    }
  }
}

int MainWindow::editorsCount() const
{
  int result = 0;

  for (int i = 0; i < this->ui->tabWidget->count(); i++) {
    IEditor *editor = qobject_cast<IEditor *>(this->ui->tabWidget->widget(i));

    if (editor != nullptr) {
      result++;
    }
  }

  return result;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
  QWidget *w = this->ui->tabWidget->widget(index);
  IEditor *editor = dynamic_cast<IEditor *> (w);
  bool cancel = false;

  if (editor != nullptr && editor->document()->changed()) {
    AppUI::CommonDialogs::DialogSaveChanges dialog(editor->document()->documentName(), this);
    dialog.exec();

    switch (dialog.answer()) {
      case AppUI::CommonDialogs::DialogSaveChanges::Save: {
        if (QFile::exists(editor->document()->documentFilename())) {
          if (editor->document()->save(editor->document()->documentFilename())) {
            this->rememberFilename(editor->document()->documentFilename());
          }
        } else {
          QFileDialog dialog(this->parentWidget());
          dialog.setAcceptMode(QFileDialog::AcceptSave);
          dialog.setFileMode(QFileDialog::AnyFile);
          dialog.setNameFilter(tr("XML Files (*.xml)"));
          dialog.setDefaultSuffix(QString("xml"));
          dialog.setWindowTitle(tr("Save file as"));

          if (editor->document()->documentFilename().isEmpty()) {
            dialog.setDirectory(Settings::FileDialogOptions::directory(Settings::FileDialogOptions::Dialogs::SaveDocument));
            dialog.selectFile(editor->document()->documentName());
          } else {
            dialog.selectFile(editor->document()->documentFilename());
          }

          if (dialog.exec() == QDialog::Accepted) {
            Settings::FileDialogOptions::setDirectory(Settings::FileDialogOptions::Dialogs::SaveDocument, dialog.directory().absolutePath());
            QString filename = dialog.selectedFiles().at(0);

            if (editor->document()->save(filename)) {
              this->rememberFilename(editor->document()->documentFilename());
            } else {
              cancel = true;
            }
          } else {
            cancel = true;
          }
        }

        break;
      }

      case AppUI::CommonDialogs::DialogSaveChanges::DontSave:
        break;

      case AppUI::CommonDialogs::DialogSaveChanges::Cancel:
        cancel = true;
        break;
    }
  }

  if (!cancel) {
    this->ui->tabWidget->removeTab(index);
    this->checkStartPageVisible();
    delete w;
  }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
  Q_UNUSED(index);
  this->updateMenuState();

  this->mStatusManager->hideAll();
  IEditor *editor = this->currentEditor();

  if (editor != nullptr) {
    IEditor *editor = this->currentEditor();
    this->mStatusManager->updateData(editor->statusData());
  }
}

void MainWindow::setCurrentTab(QWidget *widget)
{
  this->ui->tabWidget->setCurrentWidget(widget);
}

void MainWindow::actionLanguage_triggered()
{
  QAction *action = qobject_cast<QAction *>(sender());
  QString name = action->data().toString();
  this->selectLocale(name);
}

void MainWindow::updateMenuState()
{
  IEditor *editor = this->currentEditor();
  bool editorSelected = (editor != nullptr);

  if (editor != nullptr) {
    this->ui->menuFont->setEnabled(editor->type() == IEditor::EditorFont);

    this->ui->actionEditUndo->setEnabled(editor->document()->canUndo());
    this->ui->actionEditRedo->setEnabled(editor->document()->canRedo());
  } else {
    this->ui->menuFont->setEnabled(false);

    this->ui->actionEditUndo->setEnabled(false);
    this->ui->actionEditRedo->setEnabled(false);
  }

  this->ui->menuEdit->setEnabled(editorSelected);
  this->ui->menuImage->setEnabled(editorSelected);

  this->ui->actionRename->setEnabled(editorSelected);
  this->ui->actionSave->setEnabled(editorSelected);
  this->ui->actionSave_As->setEnabled(editorSelected);
  this->ui->actionClose->setEnabled(editorSelected);
  this->ui->actionClose_All->setEnabled(editorSelected && (this->editorsCount() > 1));
  this->ui->actionConvert->setEnabled(editorSelected);
  this->ui->actionConvert_All->setEnabled(editorSelected);
}

void MainWindow::updateRecentList()
{
  QList<QAction *> actions = this->ui->menuRecent->actions();
  QAction *action;

  foreach (action, actions) {
    this->ui->menuRecent->removeAction(action);
  }

  for (int i = actions.count() - 1; i >= 0; i--) {
    action = actions.at(i);
    delete action;
  }

  if (this->mRecentList->files()->count() > 0) {
    this->ui->menuRecent->setEnabled(true);

    for (int i = 0; i < this->mRecentList->files()->count(); i++) {
      QString filename = this->mRecentList->files()->at(i);
      QString strippedName = QFileInfo(filename).fileName();
      QString text = QString("%1 %2").arg(i + 1).arg(strippedName);

      action = new QAction(text, this);
      action->setData(filename);
      this->connect(action, SIGNAL(triggered()), SLOT(openRecentFile()));
      //action->setVisible(true);
      this->ui->menuRecent->addAction(action);
    }
  } else {
    this->ui->menuRecent->setEnabled(false);
  }
}

void MainWindow::openRecentFile()
{
  QAction *action = qobject_cast<QAction *>(sender());

  if (action) {
    QStringList files = QStringList() << action->data().toString();
    this->mFileHandlers->openFiles(files);
  }
}

void MainWindow::rememberFilename(const QString &filename)
{
  this->mRecentList->add(filename);
}

void MainWindow::closeRequest(QWidget *tab)
{
  int index = this->ui->tabWidget->indexOf(tab);

  if (index >= 0) {
    this->on_tabWidget_tabCloseRequested(index);
  }
}

IEditor *MainWindow::currentEditor()
{
  IEditor *result = qobject_cast<IEditor *>(this->currentTab());
  return result;
}

QWidget *MainWindow::currentTab()
{
  int index = this->ui->tabWidget->currentIndex();

  if (index >= 0) {
    QWidget *w = this->ui->tabWidget->widget(index);
    return w;
  }

  return nullptr;
}

void MainWindow::tabsList(QList<QWidget *> *list)
{
  if (list != nullptr) {
    list->clear();

    for (int i = 0; i < this->ui->tabWidget->count(); i++) {
      list->append(this->ui->tabWidget->widget(i));
    }
  }
}

QWidget *MainWindow::parentWidget()
{
  return this;
}

QString MainWindow::findAvailableName(const QString &prefix)
{
  QString result = prefix;
  QList<QString> names;

  for (int i = 0; i < this->ui->tabWidget->count(); i++) {
    QWidget *w = this->ui->tabWidget->widget(i);
    Data::Containers::IDocument *doc = dynamic_cast<Data::Containers::IDocument *> (w);

    if (doc != nullptr) {
      names.append(doc->documentName());
    }
  }

  int i = 1;

  if (names.contains(prefix)) {
    while (true) {
      QString name = QString("%1 %2").arg(prefix).arg(i++);

      if (!names.contains(name)) {
        result = name;
        break;
      }
    }
  }

  return result;
}

void MainWindow::tabChanged(QWidget *tab)
{
  int index = this->ui->tabWidget->indexOf(tab);

  if (index >= 0) {
    this->tabTextUpdate(tab);
  }
}

int MainWindow::tabCreated(QWidget *newTab)
{
  int index = this->ui->tabWidget->addTab(newTab, "");
  this->ui->tabWidget->setCurrentIndex(index);
  this->tabTextUpdate(newTab);

  this->connect(newTab, SIGNAL(statusChanged()), SLOT(statusChanged()));

  this->checkStartPageVisible();

  IEditor *editor = this->currentEditor();
  this->mStatusManager->updateData(editor->statusData());

  return index;
}

void MainWindow::statusChanged()
{
  QWidget *widget = qobject_cast<QWidget *>(sender());

  if (widget != nullptr) {
    if (this->ui->tabWidget->currentWidget() == widget) {
      IEditor *editor = this->currentEditor();
      this->mStatusManager->updateData(editor->statusData());
    }
  }
}

} // namespace AppUI
