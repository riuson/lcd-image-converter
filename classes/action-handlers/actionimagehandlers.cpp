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
#include "converterhelper.h"
#include "documentoperator.h"
#include "imageflip.h"
#include "imagerotate.h"
#include "imageshift.h"
#include "imageinverse.h"
#include "imageresize.h"
#include "imagegrayscale.h"
#include "imageimport.h"
#include "imageexport.h"
//-----------------------------------------------------------------------------
ActionImageHandlers::ActionImageHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
    this->mRunningError = false;
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipHorizontal_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageFlip imageFlip(this);
        imageFlip.setOrientation(true, false);
        docOp.apply(this->editor()->document(), imageFlip);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::flipVertical_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageFlip imageFlip(this);
        imageFlip.setOrientation(false, true);
        docOp.apply(this->editor()->document(), imageFlip);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Clockwise_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageRotate imageRotate(this);
        imageRotate.setAngle(Operations::ImageRotate::Angle::A90);
        docOp.apply(this->editor()->document(), imageRotate);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_180_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageRotate imageRotate(this);
        imageRotate.setAngle(Operations::ImageRotate::Angle::A180);
        docOp.apply(this->editor()->document(), imageRotate);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::rotate_90_Counter_Clockwise_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageRotate imageRotate(this);
        imageRotate.setAngle(Operations::ImageRotate::Angle::A270);
        docOp.apply(this->editor()->document(), imageRotate);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_left_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageShift imageShift(this);
        imageShift.setDirection(Operations::ImageShift::Direction::Left);
        docOp.apply(this->editor()->document(), imageShift);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_right_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageShift imageShift(this);
        imageShift.setDirection(Operations::ImageShift::Direction::Right);
        docOp.apply(this->editor()->document(), imageShift);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_up_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageShift imageShift(this);
        imageShift.setDirection(Operations::ImageShift::Direction::Up);
        docOp.apply(this->editor()->document(), imageShift);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::shift_down_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageShift imageShift(this);
        imageShift.setDirection(Operations::ImageShift::Direction::Down);
        docOp.apply(this->editor()->document(), imageShift);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::inverse_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageInverse imageInverse(this);
        docOp.apply(this->editor()->document(), imageInverse);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::resize_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageResize imageResize(this->mMainWindow->parentWidget(), this);
        docOp.apply(this->editor()->document(), imageResize);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::grayscale_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageGrayscale imageGrayscale(this);
        docOp.apply(this->editor()->document(), imageGrayscale);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::import_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageImport imageImport(this->mMainWindow->parentWidget(), this);
        docOp.apply(this->editor()->document(), imageImport);
    }
}
//-----------------------------------------------------------------------------
void ActionImageHandlers::export_triggered()
{
    if (this->editor() != NULL)
    {
        QStringList keys = this->editor()->selectedKeys();

        Operations::DocumentOperator docOp(this);
        docOp.setKeys(keys);
        Operations::ImageExport imageExport(this->mMainWindow->parentWidget(), this);
        docOp.apply(this->editor()->document(), imageExport);
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
