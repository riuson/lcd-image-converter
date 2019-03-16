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

#include "actionfonthandlers.h"

#include <QFileDialog>
#include <QFileInfo>
#include "widgetbitmapeditor.h"
#include "bitmaphelper.h"
#include "fonthelper.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include "dialogfontpreview.h"
#include "imainwindow.h"
#include "idocument.h"
#include "datacontainer.h"
#include "fontparameters.h"
#include "documentoperator.h"
#include "imageinverse.h"
#include "fontresize.h"

namespace AppUI
{
namespace MenuHandlers
{

ActionFontHandlers::ActionFontHandlers(QObject *parent) :
  ActionHandlersBase(parent)
{
}

void ActionFontHandlers::fontChange_triggered()
{
  if (AppUI::Fonts::EditorTabFont *etf = qobject_cast<AppUI::Fonts::EditorTabFont *>(this->mMainWindow->currentTab())) {
    QString chars;
    Data::Containers::FontParameters parameters;
    etf->fontCharacters(&chars, &parameters);

    AppUI::Fonts::DialogFontSelect dialog(this->mMainWindow->parentWidget());
    dialog.setCharacters(chars);
    dialog.setFontParameters(parameters);

    if (dialog.exec() == QDialog::Accepted) {
      QString chars = dialog.characters();

      dialog.getFontParameters(&parameters);

      etf->setFontCharacters(chars, parameters);
    }
  }
}

void ActionFontHandlers::fontInverse_triggered()
{
  if (this->editor() != nullptr) {
    Operations::DocumentOperator docOp(this);
    Operations::ImageInverse imageInverse(this);
    docOp.apply(this->editor()->document(), imageInverse);
  }
}

void ActionFontHandlers::fontResize_triggered()
{
  if (this->editor() != nullptr) {
    Operations::DocumentOperator docOp(this);
    Operations::FontResize fontResize(this->mMainWindow->parentWidget(), this);
    docOp.apply(this->editor()->document(), fontResize);
  }
}

void ActionFontHandlers::fontPreview_triggered()
{
  IEditor *editor = this->editor();

  if (editor != nullptr) {
    Data::Containers::IDocument *doc = editor->document();

    AppUI::Fonts::DialogFontPreview dialog(this->mMainWindow->parentWidget());
    dialog.setDocument(doc);

    dialog.exec();
  }
}

void ActionFontHandlers::fontToImage_triggered()
{
  IEditor *editor = this->editor();

  if (editor != nullptr) {
    QStringList keys = editor->selectedKeys();
    qSort(keys);
    QString characters = keys.join("");
    QImage image = Parsing::Conversion::FontHelper::drawString(editor->document()->dataContainer(), characters);

    emit this->imageCreated(&image, "image_" + editor->document()->documentName());
  }
}

} // namespace MenuHandlers
} // namespace AppUI
