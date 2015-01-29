#include "windoweditor.h"
#include "ui_windoweditor.h"

#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
#include <QIcon>

#include "bitmaphelper.h"
#include "bitmapeditoroptions.h"
#include "toolsmanager.h"
#include "iimageeditortool.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
WindowEditor::WindowEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowEditor)
{
    ui->setupUi(this);

    this->ui->label->installEventFilter(this);

    this->mScale = 10;
}
//-----------------------------------------------------------------------------
WindowEditor::~WindowEditor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WindowEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        this->updateImageScaled(this->mScale);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
bool WindowEditor::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->mFlagChanged = false;
        }
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        // get coordinates
        int xscaled = me->pos().x();
        int yscaled = me->pos().y();
        int xreal = xscaled / this->mScale;
        int yreal = yscaled / this->mScale;
        if (!this->mImageOriginal.isNull())
        {
            if (xreal < this->mImageOriginal.width() && yreal < this->mImageOriginal.height())
            {
                // show coordinates
                QPoint mouseCoordinates(xreal, yreal);
                emit this->mouseMove(&mouseCoordinates);

                // get buttons
                bool buttonLeft = (me->buttons() & Qt::LeftButton) == Qt::LeftButton;
                bool buttonRight = (me->buttons() & Qt::RightButton) == Qt::RightButton;

                // draw on pixmap
                if (buttonLeft)
                {
                    this->drawPixel(xreal, yreal, this->mColor1);
                    this->mFlagChanged = true;
                }
                if(buttonRight)
                {
                    this->drawPixel(xreal, yreal, this->mColor2);
                    this->mFlagChanged = true;
                }
            }
            else
            {
                QPoint mouseCoordinates(-1, -1);
                emit this->mouseMove(&mouseCoordinates);
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (this->mFlagChanged)
        {
            emit this->imageChanged();
        }
        this->mFlagChanged = false;
    }
    else
    {
        result = QObject::eventFilter(obj, event);
    }
    return result;
}
//-----------------------------------------------------------------------------
void WindowEditor::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
    {
        QPoint point = event->globalPos();
        point = this->mapFromGlobal(point);

        QRect labelRect = this->ui->label->rect();
        QPoint labelPoint = this->ui->label->pos();
        labelRect.moveTo(labelPoint);

        if (labelRect.contains(point.x(), point.y()))
        {
            if (event->orientation() == Qt::Vertical)
            {
                int scale = this->mScale;
                if (event->delta() > 0)
                    scale++;
                else
                    scale--;

                this->setScale(scale);
                //this->ui->spinBoxScale->setValue(this->mScale);
            }
            event->accept();
        }
    }
}
//-----------------------------------------------------------------------------
const QImage *WindowEditor::image() const
{
    const QImage *result = &this->mImageOriginal;
    return result;
}
//-----------------------------------------------------------------------------
void WindowEditor::setImage(const QImage *value)
{
    this->mImageOriginal = QImage(*value);
    this->updateImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
QColor WindowEditor::color1() const
{
    return this->mColor1;
}
//-----------------------------------------------------------------------------
QColor WindowEditor::color2() const
{
    return this->mColor2;
}
//-----------------------------------------------------------------------------
int WindowEditor::scale() const
{
    return this->mScale;
}
//-----------------------------------------------------------------------------
void WindowEditor::setTools(ToolsManager *tools)
{
    this->mTools = tools;

    for (int i = 0; i < this->mTools->tools()->length(); i++)
    {
        IImageEditorTool *tool = this->mTools->tools()->at(i);
        QAction *toolAction = this->ui->toolBarTools->addAction(*tool->icon(), tool->title());
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::updateImageScaled(int scale)
{
    if (!this->mImageOriginal.isNull())
    {
        this->mImageScaled = BitmapHelper::scale(&this->mImageOriginal, scale);
        this->mImageScaled = BitmapHelper::drawGrid(&this->mImageScaled, scale);
        this->mPixmapScaled = QPixmap::fromImage(this->mImageScaled);

        this->ui->label->setPixmap(this->mPixmapScaled);
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::drawPixel(int x, int y, const QColor &color)
{
    QImage image = this->mImageOriginal;
    this->mImageOriginal = BitmapHelper::drawPixel(&image, x, y, color);
    this->updateImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
void WindowEditor::on_spinBoxScale_valueChanged(int value)
{
    this->setScale(value);
}
//-----------------------------------------------------------------------------
void WindowEditor::on_pushButtonColor1_clicked()
{
    QColorDialog dialog(this->mColor1, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor1 = dialog.selectedColor();
        BitmapEditorOptions::setColor1(this->mColor1);
        this->mPixmapColor1.fill(this->mColor1);
        //this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::on_pushButtonColor2_clicked()
{
    QColorDialog dialog(this->mColor2, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor2 = dialog.selectedColor();
        BitmapEditorOptions::setColor2(this->mColor2);
        this->mPixmapColor2.fill(this->mColor2);
        //this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::setScale(int value)
{
    if (value > 0 && value <= 50)
    {
        if (this->mScale != value)
        {
            this->mScale = value;
            this->updateImageScaled(this->mScale);

            BitmapEditorOptions::setScale(value);

            emit this->scaleSchanged(this->mScale);
        }
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::setColor1(const QColor value)
{
    this->mColor1 = value;
}
//-----------------------------------------------------------------------------
void WindowEditor::setColor2(const QColor value)
{
    this->mColor2 = value;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
