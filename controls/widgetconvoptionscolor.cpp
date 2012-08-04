/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#include "widgetconvoptionscolor.h"
#include "ui_widgetconvoptionscolor.h"
//-----------------------------------------------------------------------------
#include <QButtonGroup>

#include "bytelistitemdelegate.h"
#include "convertercolor.h"
#include "converterhelper.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsColor::WidgetConvOptionsColor(IConverter *options, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsColor)
{
    ui->setupUi(this);

    this->mReady = false;

    this->mDelegate = new ByteListItemDelegate(this);
    this->mDelegate->enableHighlightColors(true);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->mConv = dynamic_cast<ConverterColor*>(options);
    if (this->mConv == NULL)
    {
        this->mConv = new ConverterColor(this);
        this->mConv->loadSettings();
    }

    int bitsPerPointRed = this->mConv->depthRed();
    int bitsPerPointGreen = this->mConv->depthGreen();
    int bitsPerPointBlue = this->mConv->depthBlue();
    ConverterColor::ColorsOrder orderColors = this->mConv->orderRGB();

    this->ui->spinBoxRedbits->setValue(bitsPerPointRed);
    this->ui->spinBoxGreenBits->setValue(bitsPerPointGreen);
    this->ui->spinBoxBlueBits->setValue(bitsPerPointBlue);

    QStringList ordersList;
    ordersList << "RGB";
    ordersList << "RBG";
    ordersList << "GRB";
    ordersList << "GBR";
    ordersList << "BRG";
    ordersList << "BGR";
    this->ui->comboBoxColorsOrder->addItems(ordersList);
    this->ui->comboBoxColorsOrder->setCurrentIndex((int)orderColors);

    this->mReady = true;
    this->updatePreview();
}
//-----------------------------------------------------------------------------
WidgetConvOptionsColor::~WidgetConvOptionsColor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::updatePreview()
{
    if (!this->mReady)
        return;

    this->ui->listWidget->clear();
    QStringList list;

    ConverterColor::DataLength length = this->mConv->length();
    int bits = (int)length;
    this->mDelegate->setBitsCount(bits);

    int bitsPerPointR = this->ui->spinBoxRedbits->value();
    int bitsPerPointG = this->ui->spinBoxGreenBits->value();
    int bitsPerPointB = this->ui->spinBoxBlueBits->value();

    QStringList colorsRed, colorsGreen, colorsBlue;
    for (int i = bitsPerPointR - 1; i >= 0; i--)
        colorsRed << QString("R.%1").arg(i);
    for (int i = bitsPerPointG - 1; i >= 0; i--)
        colorsGreen << QString("G.%1").arg(i);
    for (int i = bitsPerPointB - 1; i >= 0; i--)
        colorsBlue << QString("B.%1").arg(i);

    QString colorsOrder = this->ui->comboBoxColorsOrder->currentText();
    ConverterColor::ColorsOrder orderColors = (ConverterColor::ColorsOrder)this->ui->comboBoxColorsOrder->currentIndex();
    QStringList colors;
    for (int i = 0; i < colorsOrder.length(); i++)
    {
        if (colorsOrder.at(i) == QChar('R'))
            colors << colorsRed;
        if (colorsOrder.at(i) == QChar('G'))
            colors << colorsGreen;
        if (colorsOrder.at(i) == QChar('B'))
            colors << colorsBlue;
    }

    bool swapBytes = this->mConv->swapBytes();
    bool mirror = this->mConv->mirror();
    bool pack = this->mConv->pack();
    IConverter::DataAlign align = this->mConv->align();

    ConverterHelper::packDataPreview(&list, colors, bits, pack, (align == IConverter::AlignHigh));

    if (swapBytes)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= bits)
        {
            for (int j = (bits / 8); j > 0; j--)//1, 2 or 4
            {
                int start = i + (j - 1) * 8;
                if (start >= 0 && (start + 8 - 1) <= list.count() )
                outlist.append(list.mid(start, 8));
            }
        }
        list = outlist;
    }

    if (mirror)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= 8)
        {
            QStringList tmp = list.mid(i, 8);
            for (int j = tmp.count() - 1; j >= 0; j--)
            {
                outlist.append(tmp.at(j));
            }
        }
        list = outlist;
    }
    this->ui->listWidget->addItems(list);

    this->mConv->setDepthRed(bitsPerPointR);
    this->mConv->setDepthGreen(bitsPerPointG);
    this->mConv->setDepthBlue(bitsPerPointB);
    this->mConv->setOrderColors(orderColors);

    this->mConv->saveSettings();

    emit this->settingsChanged();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::dataLengthChanged(int value)
{
    switch (value)
    {
    case 8:
    case 16:
    case 32:
        this->mConv->setLength((IConverter::DataLength)value);
        break;
    default:
        this->mConv->setLength(IConverter::Data8);
        break;
    }
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::dataPackChanged(bool value)
{
    this->mConv->setPack(value);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::dataAlignChanged(int value)
{
    this->mConv->setAlign((IConverter::DataAlign)value);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::swapBytesChanged(bool value)
{
    this->mConv->setSwapBytes(value);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::mirrorBytesChanged(bool value)
{
    this->mConv->setMirror(value);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
