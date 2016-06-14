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
#include "tfontparameters.h"
#include "documentoperator.h"
#include "fontinverse.h"
#include "fontminimizeheight.h"
#include "fontresize.h"
//-----------------------------------------------------------------------------
ActionFontHandlers::ActionFontHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontChange_triggered()
{
    if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(this->mMainWindow->currentTab()))
    {
        QString chars;
        tFontParameters parameters;
        etf->fontCharacters(&chars, &parameters);

        DialogFontSelect dialog(this->mMainWindow->parentWidget());
        dialog.setCharacters(chars);
        dialog.setFontParameters(parameters);

        if (dialog.exec() == QDialog::Accepted)
        {
            QString chars = dialog.characters();

            dialog.getFontParameters(&parameters);

            etf->setFontCharacters(chars, parameters);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontInverse_triggered()
{
    if (this->editor() != NULL)
    {
        Operations::DocumentOperator docOp(this);
        Operations::FontInverse fontInverse(this);
        docOp.apply(this->editor()->document(), fontInverse);
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontResize_triggered()
{
    if (this->editor() != NULL)
    {
        Operations::DocumentOperator docOp(this);
        Operations::FontResize fontResize(this->mMainWindow->parentWidget(), this);
        docOp.apply(this->editor()->document(), fontResize);
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontMinimizeHeight_triggered()
{
    if (this->editor() != NULL)
    {
        Operations::DocumentOperator docOp(this);
        Operations::FontMinimizeHeight fontMinimizeHeight(this->mMainWindow->parentWidget(), this);
        docOp.apply(this->editor()->document(), fontMinimizeHeight);
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontPreview_triggered()
{
    IEditor *editor = this->editor();
    if (editor != NULL)
    {
        IDocument *doc = editor->document();

        DialogFontPreview dialog(this->mMainWindow->parentWidget());
        dialog.setDocument(doc);

        dialog.exec();
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontToImage_triggered()
{
    IEditor *editor = this->editor();
    if (editor != NULL)
    {
        QStringList keys = editor->selectedKeys();
        qSort(keys);
        QString characters = keys.join("");
        QImage image = FontHelper::drawString(editor->document()->dataContainer(), characters);

        emit this->imageCreated(&image, "image_" + editor->document()->documentName());
    }
}
//-----------------------------------------------------------------------------
