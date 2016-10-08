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

#include "toolmove.h"
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QToolButton>
#include <QSettings>
#include "bitmaphelper.h"
#include "iimageeditorparams.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolMove::ToolMove(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
    this->mParameters = parameters;
    this->mIcon = new QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_select_move"), 24)));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();

    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolMove::~ToolMove()
{
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolMove::title() const
{
    return tr("Move");
}
//-----------------------------------------------------------------------------
const QString ToolMove::tooltip() const
{
    return tr("Move entire image or selection");
}
//-----------------------------------------------------------------------------
const QIcon *ToolMove::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolMove::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolMove::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
bool ToolMove::processMouse(QMouseEvent *event,
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
                Qt::MouseButtons buttons = event->buttons();
                int x = event->x();
                int y = event->y();

                switch (this->mToolMode)
                {
                case MoveCut:
                    this->processMoveCut(buttons, x, y);
                    break;
                case MoveCopy:
                    this->processMoveCopy(buttons, x, y);
                    break;
                case MoveCircular:
                    this->processMoveCircular(buttons, x, y);
                    break;
                }
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        //emit this->completed(&this->mOriginalImage, false);
    }

    return true;
}
//-----------------------------------------------------------------------------
void ToolMove::initializeWidgets()
{
    QActionGroup *group = new QActionGroup(this);

    this->mActionMoveCut = new QAction(this);
    this->mActionMoveCut->setCheckable(true);
    this->mActionMoveCut->setToolTip(tr("Cut & Move"));
    this->mActionMoveCut->setIcon(QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_move_cut"), 24))));
    this->connect(this->mActionMoveCut, SIGNAL(triggered()), SLOT(on_switchToMoveCut()));
    this->mActions->append(this->mActionMoveCut);
    group->addAction(this->mActionMoveCut);

    this->mActionMoveCopy = new QAction(this);
    this->mActionMoveCopy->setCheckable(true);
    this->mActionMoveCopy->setToolTip(tr("Copy & Move"));
    this->mActionMoveCopy->setIcon(QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_move_copy"), 24))));
    this->connect(this->mActionMoveCopy, SIGNAL(triggered()), SLOT(on_switchToMoveCopy()));
    this->mActions->append(this->mActionMoveCopy);
    group->addAction(this->mActionMoveCopy);

    this->mActionMoveCircular = new QAction(this);
    this->mActionMoveCircular->setCheckable(true);
    this->mActionMoveCircular->setToolTip(tr("Move circular"));
    this->mActionMoveCircular->setIcon(QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_move_circular"), 24))));
    this->connect(this->mActionMoveCircular, SIGNAL(triggered()), SLOT(on_switchToMoveCircular()));
    this->mActions->append(this->mActionMoveCircular);
    group->addAction(this->mActionMoveCircular);

    this->mActionMoveCut->setChecked(true);
    this->mToolMode = MoveCut;
}
//-----------------------------------------------------------------------------
void ToolMove::processMoveCut(Qt::MouseButtons buttons, int x, int y)
{
}
//-----------------------------------------------------------------------------
void ToolMove::processMoveCopy(Qt::MouseButtons buttons, int x, int y)
{
    /*
    if (this->mSelectedPath.isEmpty())
    {
        return;
    }

    if ((buttons & Qt::LeftButton) == Qt::LeftButton)
    {
        if (!this->mFlagChanged)
        {
            this->mStartPoint = QPoint(x, y);
            this->mSelectedPathInternal = this->mSelectedPath;
        }

        int x1 = this->mStartPoint.x();
        int y1 = this->mStartPoint.y();
        int x2 = x;
        int y2 = y;

        QPainterPath path = this->mSelectedPathInternal;
        QPoint offset = QPoint(x2 - x1, y2 - y1);
        this->mSelectedPath = path.translated(offset);

        this->mFlagChanged = true;
        emit this->selectionChanged(this->mSelectedPath);
    }
    */
}
//-----------------------------------------------------------------------------
void ToolMove::processMoveCircular(Qt::MouseButtons buttons, int x, int y)
{

}
//-----------------------------------------------------------------------------
void ToolMove::on_switchToMoveCut()
{
    this->mToolMode = MoveCut;
}
//-----------------------------------------------------------------------------
void ToolMove::on_switchToMoveCopy()
{
    this->mToolMode = MoveCopy;
}
//-----------------------------------------------------------------------------
void ToolMove::on_switchToMoveCircular()
{
    this->mToolMode = MoveCircular;
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
