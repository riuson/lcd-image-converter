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
#include "dialogresize.h"
#include "bitmaphelper.h"
#include "fonthelper.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include "dialogfontpreview.h"
#include "imainwindow.h"
#include "idocument.h"
#include "datacontainer.h"
#include "limits"
#include "bitmapeditoroptions.h"
#include "dialogcanvasresize.h"
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
        QString chars, fontFamily, style;
        int size;
        bool monospaced, antialiasing;
        etf->fontCharacters(&chars, &fontFamily, &style, &size, &monospaced, &antialiasing);

        DialogFontSelect dialog(this->mMainWindow->parentWidget());
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

            this->document()->beginChanges();

            etf->setFontCharacters(chars, family, style, size, monospaced, antialiasing);

            this->document()->endChanges();
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontInverse_triggered()
{
    if (this->document() != NULL)
    {
        this->document()->beginChanges();;

        QStringList keys = this->document()->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            const QImage *original = this->document()->dataContainer()->image(key);
            QImage result(*original);
            result.invertPixels();
            this->document()->dataContainer()->setImage(key, &result);
        }

        this->document()->endChanges();
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontResize_triggered()
{
    if (this->document() != NULL)
    {
        QStringList keys = this->document()->dataContainer()->keys();

        DialogCanvasResize dialog(this->document()->dataContainer(), this->mMainWindow->parentWidget());
        //dialog.selectKeys(keys);

        if (dialog.exec() == QDialog::Accepted)
        {
            int left, top, right, bottom;
            dialog.getResizeInfo(&left, &top, &right, &bottom);

            if (left != 0 || top != 0 || right != 0 || bottom != 0)
            {
                this->document()->beginChanges();

                QStringListIterator iterator(keys);
                while (iterator.hasNext())
                {
                    QString key = iterator.next();

                    const QImage *original = this->document()->dataContainer()->image(key);
                    QImage result = BitmapHelper::crop(original, left, top, right, bottom, BitmapEditorOptions::color2());
                    this->document()->dataContainer()->setImage(key, &result);
                }

                this->document()->endChanges();
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontMinimizeHeight_triggered()
{
    IDocument *doc = this->document();
    if (doc != NULL)
    {
        int left = std::numeric_limits<int>::max();
        int top = std::numeric_limits<int>::max();
        int right = 0;
        int bottom = 0;
        int l, t, r, b;
        int width = 0;
        int height = 0;

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
            right = qMax(right, r);
            bottom = qMax(bottom, b);

            width = qMax(width, original->width());
            height = qMax(height, original->height());
        }

        DialogResize dialog(width, bottom + 1 - top, 0, -top, false, false, true, this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);

            this->document()->beginChanges();

            QStringList keys = doc->dataContainer()->keys();
            QListIterator<QString> it(keys);
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                const QImage *original = doc->dataContainer()->image(key);

                QImage result = BitmapHelper::resize(original, original->width(), height, width, offsetY, center, changeWidth, changeHeight, BitmapEditorOptions::color2());

                doc->dataContainer()->setImage(key, &result);
            }

            this->document()->endChanges();
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontPreview_triggered()
{
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        IDocument *doc = this->mMainWindow->currentDocument();

        DialogFontPreview dialog(this->mMainWindow->parentWidget());
        dialog.setDocument(doc);

        dialog.exec();
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::fontToImage_triggered()
{
    EditorTabFont *editor = qobject_cast<EditorTabFont *>(this->mMainWindow->currentTab());
    if (editor != NULL)
    {
        QString characters = editor->selectedCharacters();
        QImage image = FontHelper::drawString(editor->dataContainer(), characters);

        emit this->imageCreated(&image, "image_" + editor->documentName());
    }
}
//-----------------------------------------------------------------------------
