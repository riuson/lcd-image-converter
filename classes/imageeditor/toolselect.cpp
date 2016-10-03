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

#include "toolselect.h"
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QSettings>
#include <QColorDialog>
#include <QDebug>
#include "bitmaphelper.h"
#include "iimageeditorparams.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolSelect::ToolSelect(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
    this->mParameters = parameters;
    this->mIcon = new QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_select"), 24)));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();

    this->loadSettings();
    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolSelect::~ToolSelect()
{
    this->saveSettings();
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolSelect::title() const
{
    return tr("Select");
}
//-----------------------------------------------------------------------------
const QString ToolSelect::tooltip() const
{
    return tr("Select area");
}
//-----------------------------------------------------------------------------
const QIcon *ToolSelect::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolSelect::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolSelect::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
bool ToolSelect::processMouse(QMouseEvent *event,
                              const QImage *imageOriginal)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->mFlagChanged = false;
        }

        // get coordinates
        if (imageOriginal != NULL)
        {
            if (event->x() < imageOriginal->width() && event->y() < imageOriginal->height())
            {
                // operation
                Operation op = ToolSelect::None;

                if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
                {
                    op = ToolSelect::Append;
                }

                if ((event->buttons() & Qt::RightButton) == Qt::RightButton)
                {
                    op = ToolSelect::Subtract;
                }

                if (
                        ((event->buttons() & Qt::RightButton) == Qt::RightButton) &&
                        ((event->buttons() & Qt::LeftButton) == Qt::LeftButton))
                {
                    op = ToolSelect::Reset;
                }

                if ((event->buttons() & Qt::MiddleButton) == Qt::MiddleButton)
                {
                    op = ToolSelect::Reset;
                }

                // draw on pixmap
                if (op != ToolSelect::None)
                {
                    if (!this->mFlagChanged)
                    {
                        this->mOriginalImage = *imageOriginal;
                        this->mStartPoint = event->pos();
                    }

                    int x1 = this->mStartPoint.x();
                    int y1 = this->mStartPoint.y();
                    int x2 = event->x();
                    int y2 = event->y();

                    if (x2 < x1)
                        qSwap(x1, x2);

                    if (y2 < y1)
                        qSwap(y1, y2);

                    QRect rect;
                    rect.setCoords(x1, y1, x2, y2);

                    this->modifySelection(rect, op);

                    this->mFlagChanged = true;
                    emit this->processing(&this->mInternalImage);
                }
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        emit this->completed(&this->mOriginalImage, false);
        emit this->selectionChanged(&this->mSelectedPath);
    }

    return true;
}
//-----------------------------------------------------------------------------
void ToolSelect::initializeWidgets()
{
    QSpinBox *spinBoxSize = new QSpinBox();
    spinBoxSize->setMinimum(1);
    spinBoxSize->setSuffix(QString("px"));
    spinBoxSize->setToolTip(tr("Line Width"));
    this->connect(spinBoxSize, SIGNAL(valueChanged(int)), SLOT(on_spinBoxSize_valueChanged(int)));
    this->mWidgets->append(spinBoxSize);
}
//-----------------------------------------------------------------------------
void ToolSelect::loadSettings()
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("select");

    //bool ok;
    //int a = sett.value("size", QVariant(1)).toInt(&ok);

    //if (ok)
    {
        //this->mSize = a;
    }

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolSelect::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("select");

    //sett.setValue("size", QVariant(this->mSize));

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolSelect::modifySelection(const QRect &rect, Operation op)
{
    switch (op)
    {
    case ToolSelect::Append:
    {
        QPainterPath newPath;// = this->mSelectedPath;
        newPath.addRect(rect);
        newPath += this->mSelectedPath;
        this->mSelectedPath = newPath.simplified();
        break;
    }
    case ToolSelect::Subtract:
    {
        QPainterPath path;
        path.addRect(rect);
        QPainterPath newPath = this->mSelectedPath.subtracted(path);
        this->mSelectedPath = newPath.simplified();
        break;
    }
    case ToolSelect::Reset:
    {
        this->mSelectedPath = QPainterPath();
        break;
    }
    default:
    {
        break;
    }
    }

    QImage image = this->mOriginalImage;
    QPixmap pixmap = QPixmap::fromImage(image);
    QPainter painter(&pixmap);

    QColor selectionColor = QColor(255, 255, 255);
    QBrush selectionBrush(selectionColor);

    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, false);
    painter.setPen(selectionColor);
    painter.setBrush(selectionBrush);
    //painter.drawPath(this->mSelectedPath);//, selectionBrush);
    painter.fillPath(this->mSelectedPath, selectionBrush);

    this->mInternalImage = pixmap.toImage();
}
//-----------------------------------------------------------------------------
void ToolSelect::on_spinBoxSize_valueChanged(int value)
{
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
