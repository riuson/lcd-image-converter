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
#include "dialogcanvasresize.h"
#include "bitmaphelper.h"
#include "externaltooloptions.h"
#include "imainwindow.h"
#include "idocument.h"
#include "ieditor.h"
#include "datacontainer.h"
#include "bitmapeditoroptions.h"
#include "converterhelper.h"
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
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::flipHorizontal(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipVertical_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::flipVertical(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Clockwise_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::rotate90(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_180_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::rotate180(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Counter_Clockwise_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::rotate270(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_left_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::shiftLeft(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_right_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::shiftRight(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_up_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::shiftUp(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_down_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result = BitmapHelper::shiftDown(original);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::inverse_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result(*original);
            result.invertPixels();
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::resize_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

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
                    QImage result = BitmapHelper::crop(original, left, top, right, bottom, BitmapEditorOptions::color2());
                    this->editor()->document()->dataContainer()->setImage(key, &result);
                }

                this->editor()->document()->endChanges(false);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::grayscale_triggered()
{
    if (this->editor() != NULL)
    {
        this->editor()->document()->beginChanges();

        QStringList keys = this->editor()->selectedKeys();

        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            const QImage *original = this->editor()->document()->dataContainer()->image(key);
            QImage result(*original);
            ConverterHelper::makeGrayscale(result);
            this->editor()->document()->dataContainer()->setImage(key, &result);
        }

        this->editor()->document()->endChanges(false);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::import_triggered()
{
    if (this->editor() != NULL)
    {
        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
        dialog.setWindowTitle(tr("Open image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            QStringList filenames = dialog.selectedFiles();

            this->editor()->document()->beginChanges();

            QStringList keys = this->editor()->selectedKeys();

            if (filenames.length() == 1)
            {
                QStringListIterator iterator(keys);
                while (iterator.hasNext())
                {
                    QString key = iterator.next();

                    QImage imageLoaded;
                    imageLoaded.load(filenames.at(0));
                    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

                    this->editor()->document()->dataContainer()->setImage(key, &imageConverted);
                }
            }
            else if (filenames.length() > 1)
            {
                for (int i = 0; i < keys.length() && i < filenames.length(); i++)
                {
                    QImage imageLoaded;
                    imageLoaded.load(filenames.at(i));
                    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

                    this->editor()->document()->dataContainer()->setImage(keys.at(i), &imageConverted);
                }
            }

            this->editor()->document()->endChanges(false);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::export_triggered()
{
    if (this->editor() != NULL)
    {
        QFileDialog dialog(this->mMainWindow->parentWidget());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString filter = QString("Windows Bitmap (*.bmp);;" \
                            "Joint Photographic Experts Group (*.jpg *.jpeg);;" \
                            "Portable Network Graphics (*.png);;" \
                            "Portable Pixmap (*.ppm);;" \
                            "Tagged Image File Format (*.tiff);;" \
                            "X11 Bitmap (*.xbm);;" \
                            "X11 Bitmap (*.xpm)");
        dialog.setNameFilter(filter);
        dialog.setWindowTitle(tr("Save image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            filter = dialog.selectedNameFilter();
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
            this->saveImages(filename, ext);
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::edit_in_external_tool_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        QMap<QString, QString> files;

        // create map of file names
        int counter = 0;
        QStringListIterator iterator(keys);
        while (iterator.hasNext())
        {
            QString key = iterator.next();
            QDateTime time = QDateTime::currentDateTime();

            QString filename = QDir::tempPath();
            filename += "/";
            filename += time.toString("yyyy-MM-dd-hh-mm-ss-zzz");
            filename += "-";
            filename += QString("%1").arg(counter++, 10, 10, QChar('0'));
            filename += ".png";

            files.insert(key, filename);
        }

        // time of last file
        QDateTime lastTime = QDateTime::currentDateTime();

        // save images to files
        iterator.toFront();
        while (iterator.hasNext())
        {
            QString key = iterator.next();

            // get image
            const QImage *image = this->editor()->document()->dataContainer()->image(key);

            // save image to file
            QString filename = files.value(key);
            image->save(filename);

            // get last time
            QFileInfo info(filename);
            lastTime = info.lastModified();
        }


        // fill parameters list
        QStringList parameters;
        iterator.toFront();
        while (iterator.hasNext())
        {
            QString key = iterator.next();
            QString filename = files.value(key);
            parameters << filename;
        }

        // run external application with this files list as parameter
        QProcess process(this);
        this->connect(&process, SIGNAL(error(QProcess::ProcessError)), SLOT(process_error(QProcess::ProcessError)));
        this->mRunningError = false;
        process.start(ExternalToolOptions::imageEditor(), parameters);

        // wait for external application finished
        do
        {
            process.waitForFinished();
        } while (process.state() == QProcess::Running);

        if (!this->mRunningError)
        {
            // find changes
            QStringList changedKeys;

            iterator.toFront();
            while (iterator.hasNext())
            {
                QString key = iterator.next();
                QString filename = files.value(key);

                QFileInfo info(filename);

                // if file was modified
                if (info.lastModified() > lastTime)
                {
                    changedKeys << key;
                }
                else
                {
                    QFile::remove(filename);
                }
            }

            // load changed files
            if (changedKeys.length() > 0)
            {
                this->editor()->document()->beginChanges();

                iterator = QStringListIterator(changedKeys);
                while (iterator.hasNext())
                {
                    QString key = iterator.next();
                    QString filename = files.value(key);

                    QImage imageLoaded;
                    imageLoaded.load(filename);
                    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);
                    this->editor()->document()->dataContainer()->setImage(key, &imageConverted);

                    QFile::remove(filename);
                }

                this->editor()->document()->endChanges(false);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::saveImages(const QString &filename, const QString &ext)
{
    QStringList keys = this->editor()->selectedKeys();

    if (keys.length() >= 1)
    {
        QString filename2 = filename;

        QFileInfo info(filename2);

        QString fileExt = info.suffix().toLower();

        if (fileExt.isEmpty() || fileExt != ext)
        {
            filename2 += "." + ext;
        }

        if (keys.length() > 1)
        {
            filename2.chop(ext.length() + 1);

            int counter = 0;
            QStringListIterator iterator(keys);
            while (iterator.hasNext())
            {
                QString key = iterator.next();

                QString filename3 = filename2;
                filename3 += ".";
                filename3 += QString("%1").arg(counter++, 10, 10, QChar('0'));
                filename3 += ".";
                filename3 += ext;

                this->editor()->document()->dataContainer()->image(key)->save(filename3);
            }
        }
        else
        {
            this->editor()->document()->dataContainer()->image(keys.at(0))->save(filename2);
        }
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
