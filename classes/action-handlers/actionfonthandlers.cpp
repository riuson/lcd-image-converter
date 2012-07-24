#include "actionfonthandlers.h"

#include <QFileDialog>
#include <QFileInfo>
#include "widgetbitmapeditor.h"
#include "dialogresize.h"
#include "bitmaphelper.h"
#include "editortabfont.h"
#include "dialogfontselect.h"
#include "dialogfontpreview.h"
#include "imainwindow.h"
#include "idocument.h"
#include "idatacontainer.h"
#include "limits"
//-----------------------------------------------------------------------------
ActionFontHandlers::ActionFontHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::on_actionFontChange_triggered()
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

            etf->setFontCharacters(chars, family, style, size, monospaced, antialiasing);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::on_actionFontInverse_triggered()
{
    if (this->editor() != NULL)
    {
        WidgetBitmapEditor *editor = this->editor();

        QStringList keys = editor->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            QImage *original = editor->dataContainer()->image(key);
            QImage result(*original);
            result.invertPixels();
            editor->dataContainer()->setImage(key, &result);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::on_actionFontResize_triggered()
{
    if (this->editor() != NULL)
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        DialogResize dialog(original->width(), original->height(), 0, 0, true, true, false, this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);

            QStringList keys = editor->dataContainer()->keys();
            QListIterator<QString> it(keys);
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                original = editor->dataContainer()->image(key);

                QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, changeWidth, changeHeight, editor->color2());

                editor->dataContainer()->setImage(key, &result);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::on_actionFontMinimizeHeight_triggered()
{
    if (this->editor() != NULL)
    {
        WidgetBitmapEditor *editor = this->editor();

        int left = std::numeric_limits<int>::max();
        int top = std::numeric_limits<int>::max();
        int right = 0;
        int bottom = 0;
        int l, t, r, b;
        int width = 0;
        int height = 0;

        // find limits
        QStringList keys = editor->dataContainer()->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while (it.hasNext())
        {
            QString key = it.next();
            QImage *original = editor->dataContainer()->image(key);

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

            QStringList keys = editor->dataContainer()->keys();
            QListIterator<QString> it(keys);
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                QImage *original = editor->dataContainer()->image(key);

                QImage result = BitmapHelper::resize(original, original->width(), height, width, offsetY, center, changeWidth, changeHeight, editor->color2());

                editor->dataContainer()->setImage(key, &result);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionFontHandlers::on_actionFontPreview_triggered()
{
    if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(this->mMainWindow->currentTab()))
    {
        IDocument *doc = this->mMainWindow->currentDocument();

        DialogFontPreview dialog(this->mMainWindow->parentWidget());
        dialog.setDocument(doc);

        dialog.exec();
    }
}
//-----------------------------------------------------------------------------
