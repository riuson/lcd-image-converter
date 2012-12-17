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

#include "actionimagehandlers.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include "widgetbitmapeditor.h"
#include "dialogresize.h"
#include "bitmaphelper.h"
#include "externaltooloptions.h"
#include "imainwindow.h"
#include "idocument.h"
#include "datacontainer.h"
//-----------------------------------------------------------------------------
ActionImageHandlers::ActionImageHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipHorizontal_triggered()
{
    if (this->editor() != NULL)
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::flipHorizontal(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipVertical_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::flipVertical(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Clockwise_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::rotate90(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_180_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::rotate180(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Counter_Clockwise_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::rotate270(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_left_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::shiftLeft(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_right_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::shiftRight(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_up_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::shiftUp(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_down_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result = BitmapHelper::shiftDown(original);
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::inverse_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        QImage result(*original);
        result.invertPixels();
        editor->setImageChanged(&result);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::resize_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        const QImage *original = editor->image();
        DialogResize dialog(original->width(), original->height(), 0, 0, false, true, true, this->mMainWindow->parentWidget());
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center, changeWidth, changeHeight;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center, &changeWidth, &changeHeight);
            QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, changeWidth, changeHeight, editor->color2());
            editor->setImageChanged(&result);
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
            editor->setImageChanged(&image);
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
            editor->image()->save(filename);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::edit_in_external_tool_triggered()
{
    if (this->editor())
    {
        WidgetBitmapEditor *editor = this->editor();

        // prepare temporary file name
        QDateTime time = QDateTime::currentDateTime();
        QString filename = QDir::tempPath() + "/" + time.toString("yyyy-MM-dd-hh-mm-ss-zzz") + ".png";

        // save current image to file
        editor->image()->save(filename);

        // remember timestamp
        QFileInfo info(filename);
        QDateTime lastModified = info.lastModified();

        // run external application with this file as parameter
        QProcess process(this);
        this->connect(&process, SIGNAL(error(QProcess::ProcessError)), SLOT(process_error(QProcess::ProcessError)));
        this->mRunningError = false;
        process.start(ExternalToolOptions::imageEditor(), QStringList() << filename);

        // wait for external application finished
        do {
            process.waitForFinished();
        } while (process.state() == QProcess::Running);

        if (!this->mRunningError)
        {
            info.refresh();
            // if file was modified
            if (info.lastModified() > lastModified)
            {
                // load file back
                QImage image;
                image.load(filename);
                editor->setImageChanged(&image);
            }
        }
        // remove temprorary file
        QFile::remove(filename);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::process_error(QProcess::ProcessError error)
{
    this->mRunningError = true;

    QString message, description;

    switch (error)
    {
    case QProcess::FailedToStart:
        message = tr("Failed to Start");
        description = tr("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
        break;
    case QProcess::Crashed:
        message = tr("Crashed");
        description = tr("The process crashed some time after starting successfully.");
        break;
    //case QProcess::Timedout:
    //    message = tr("Timedout");
    //    description = tr("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
    //    break;
    case QProcess::ReadError:
        message = tr("Read Error");
        description = tr("An error occurred when attempting to read from the process. For example, the process may not be running.");
        break;
    case QProcess::WriteError:
        message = tr("Write Error");
        description = tr("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
        break;
    case QProcess::UnknownError:
    default:
        message = tr("Unknown Error");
        description = tr("An unknown error occurred.");
        break;
    }

    QMessageBox box(this->mMainWindow->parentWidget());
    box.setTextFormat(Qt::RichText);
    box.setText(QString("<b>%1</b>: \"%2\"<br/>%3").arg(message, ExternalToolOptions::imageEditor(), description));
    box.setWindowTitle(tr("Error running external tool"));
    box.exec();
}
//-----------------------------------------------------------------------------
