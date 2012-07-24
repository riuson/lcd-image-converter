#include "actionimagehandlers.h"

#include <QFileDialog>
#include <QFileInfo>
#include "widgetbitmapeditor.h"
#include "dialogresize.h"
#include "bitmaphelper.h"
#include "imainwindow.h"
#include "idocument.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
ActionImageHandlers::ActionImageHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipHorizontal_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result = BitmapHelper::flipHorizontal(original);
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipVertical_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result = BitmapHelper::flipVertical(original);
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Clockwise_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate90(original);
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_180_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate180(original);
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Counter_Clockwise_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result = BitmapHelper::rotate270(original);
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::inverse_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        QImage result(*original);
        result.invertPixels();
        editor->dataContainer()->setImage(key, &result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::resize_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QString key = editor->currentImageKey();
        QImage *original = editor->dataContainer()->image(key);
        DialogResize dialog(original->width(), original->height(), 0, 0, false, true, true, this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);
            QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, changeWidth, changeHeight, editor->color2());
            editor->dataContainer()->setImage(key, &result);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::import_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
        dialog.setWindowTitle(tr("Open image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            QImage image;
            image.load(dialog.selectedFiles().at(0));
            QString key = editor->currentImageKey();
            editor->dataContainer()->setImage(key, &image);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::export_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        QFileDialog dialog(this->mMainWindow->parentWidget());
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
            QString key = editor->currentImageKey();
            editor->dataContainer()->image(key)->save(filename);
        }
    }
}
//-----------------------------------------------------------------------------
