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
#include "limits"
#include "dialogcanvasresize.h"
#include "tfontparameters.h"
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
        this->editor()->document()->beginChanges();;

        QStringList keys = this->editor()->document()->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result(*original);
            result.invertPixels();
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontResize_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->document()->dataContainer()->keys();

        DialogCanvasResize dialog(this->editor()->document()->dataContainer(), this->mMainWindow->parentWidget());
        dialog.selectKeys(keys);

        if (dialog.exec() == QDialog::Accepted)
        {
            int left, top, right, bottom;
            dialog.resizeInfo(&left, &top, &right, &bottom);

            if (left != 0 || top != 0 || right != 0 || bottom != 0)
            {
                this->editor()->document()->beginChanges();

                QStringListIterator iterator(keys);
                while (iterator.hasNext())
                {
                    QString key = iterator.next();

                    const QImage *original = this->editor()->document()->dataContainer()->image(key);
                    QImage result = BitmapHelper::crop(original, left, top, right, bottom, BitmapHelper::detectBackgroundColor(original));
                    this->editor()->document()->dataContainer()->setImage(key, &result);
                }

                this->editor()->document()->endChanges(false);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontMinimizeHeight_triggered()
{
    if (this->editor() != NULL)
    {
        IDocument *doc = this->editor()->document();

        int left = std::numeric_limits<int>::max();
        int top = std::numeric_limits<int>::max();
        int right = 0;
        int bottom = 0;
        int l, t, r, b;

        // find limits
        QStringList keys = doc->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            const QImage *original = doc->dataContainer()->image(key);

            BitmapHelper::findEmptyArea(original, &l, &t, &r, &b);

            left = qMin(left, l);
            top = qMin(top, t);
            right = qMin(right, r);
            bottom = qMin(bottom, b);
        }

        DialogCanvasResize dialog(doc->dataContainer(), this->mMainWindow->parentWidget());
        dialog.selectKeys(keys);
        dialog.setResizeInfo(-left, -top, -right, -bottom);
        if (dialog.exec() == QDialog::Accepted)
        {
            dialog.resizeInfo(&left, &top, &right, &bottom);

            if (left != 0 || top != 0 || right != 0 || bottom != 0)
            {
                doc->beginChanges();

                QStringListIterator iterator(keys);
                while (iterator.hasNext())
                {
                    QString key = iterator.next();

                    const QImage *original = doc->dataContainer()->image(key);
                    QImage result = BitmapHelper::crop(original, left, top, right, bottom, BitmapHelper::detectBackgroundColor(original));
                    doc->dataContainer()->setImage(key, &result);
                }

                doc->endChanges(false);
            }
        }
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
