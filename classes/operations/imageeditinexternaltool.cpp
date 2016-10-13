/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "imageeditinexternaltool.h"
#include <QImage>
#include <QMessageBox>
#include <QMap>
#include <QDateTime>
#include <QDir>
#include "idocument.h"
#include "datacontainer.h"
#include "externaltooloptions.h"

namespace Operations {

ImageEditInExternalTool::ImageEditInExternalTool(QWidget *parentWidget, QObject *parent) :
    QObject(parent)
{
    this->mParentWidget = parentWidget;
    this->mRunningError = false;
}

bool ImageEditInExternalTool::prepare(const IDocument *doc, const QStringList &keys)
{
    Q_UNUSED(doc)
    Q_UNUSED(keys)

    return true;
}

void ImageEditInExternalTool::applyDocument(IDocument *doc, const QStringList &keys)
{
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
        const QImage *image = doc->dataContainer()->image(key);

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
    this->connect(&process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
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
            iterator = QStringListIterator(changedKeys);
            while (iterator.hasNext())
            {
                QString key = iterator.next();
                QString filename = files.value(key);

                QImage imageLoaded;
                imageLoaded.load(filename);
                QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);
                doc->dataContainer()->setImage(key, &imageConverted);

                QFile::remove(filename);
            }
        }
    }
}

void ImageEditInExternalTool::applyItem(IDocument *doc, const QString &itemKey)
{
    Q_UNUSED(doc)
    Q_UNUSED(itemKey)
}

void ImageEditInExternalTool::processError(QProcess::ProcessError error)
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

    QMessageBox box(this->mParentWidget);
    box.setTextFormat(Qt::RichText);
    box.setText(QString("<b>%1</b>: \"%2\"<br/>%3").arg(message, ExternalToolOptions::imageEditor(), description));
    box.setWindowTitle(tr("Error running external tool"));
    box.exec();
}

}
