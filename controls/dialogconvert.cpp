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

#include "dialogconvert.h"
#include "ui_dialogconvert.h"

#include "converter.h"
#include "idatacontainer.h"
#include "bitmapdata.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);

    this->mWidgetSetup = NULL;
    this->mData = dataContainer;

    this->ui->comboBoxDataLength->addItem(tr("8 bit"), QVariant(8));
    this->ui->comboBoxDataLength->addItem(tr("16 bit"), QVariant(16));
    this->ui->comboBoxDataLength->addItem(tr("32 bit"), QVariant(32));

    this->mConverter = new Converter(this);
    QString name = this->mConverter->name();
    QStringList names = this->mConverter->names();
    for (int i = 0; i < names.count(); i++)
    {
        IConverter *conv = this->mConverter->conv(names.at(i));
        this->ui->comboBoxType->addItem(conv->displayName(), QVariant(conv->name()));
    }

    if (names.contains(name))
        this->ui->comboBoxType->setCurrentIndex(names.indexOf(name));
    else
        this->ui->comboBoxType->setCurrentIndex(0);

    if (this->mData != NULL)
    {
        this->ui->comboBoxSampleKey->addItems(this->mData->keys());
        this->ui->comboBoxSampleKey->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBoxSampleKey->setEnabled(false);
    }

    this->ui->checkBoxMirrorBytes->setChecked(this->mConverter->mirror());
    this->ui->checkBoxPack->setChecked(this->mConverter->pack());
    this->ui->checkBoxSwapBytes->setChecked(this->mConverter->swapBytes());
    switch (this->mConverter->length())
    {
    case IConverter::Data8:
        this->ui->comboBoxDataLength->setCurrentIndex(0);
        break;
    case IConverter::Data16:
        this->ui->comboBoxDataLength->setCurrentIndex(1);
        break;
    case IConverter::Data32:
        this->ui->comboBoxDataLength->setCurrentIndex(2);
        break;
    }

    int transform = this->mConverter->transform();

    this->ui->comboBoxRotate->addItem(tr("Rotate none"), QVariant(BitmapHelper::None));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Clockwise"), QVariant(BitmapHelper::Rotate90));
    this->ui->comboBoxRotate->addItem(tr("Rotate 180"), QVariant(BitmapHelper::Rotate180));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Counter-Clockwise"), QVariant(BitmapHelper::Rotate270));

    this->ui->comboBoxRotate->setCurrentIndex(0);

    if ((transform & 0x03) == BitmapHelper::Rotate90)
        this->ui->comboBoxRotate->setCurrentIndex(1);

    if ((transform & 0x03) == BitmapHelper::Rotate180)
        this->ui->comboBoxRotate->setCurrentIndex(2);

    if ((transform & 0x03) == BitmapHelper::Rotate270)
        this->ui->comboBoxRotate->setCurrentIndex(3);

    if ((transform & BitmapHelper::FlipHorizontal) == BitmapHelper::FlipHorizontal)
        this->ui->checkBoxFlipHorizontal->setChecked(true);
    else
        this->ui->checkBoxFlipHorizontal->setChecked(false);

    if ((transform & BitmapHelper::FlipVertical) == BitmapHelper::FlipVertical)
        this->ui->checkBoxFlipVertical->setChecked(true);
    else
        this->ui->checkBoxFlipVertical->setChecked(false);

    if ((transform & BitmapHelper::Inverse) == BitmapHelper::Inverse)
        this->ui->checkBoxInverse->setChecked(true);
    else
        this->ui->checkBoxInverse->setChecked(false);

    if (this->mConverter->align() == IConverter::AlignHigh)
        this->ui->radioButtonAlignHigh->setChecked(true);
    else
        this->ui->radioButtonAlignLow->setChecked(true);

    this->connect(this->ui->checkBoxMirrorBytes, SIGNAL(toggled(bool)), SIGNAL(mirrorBytesChanged(bool)));
    this->connect(this->ui->checkBoxSwapBytes, SIGNAL(toggled(bool)), SIGNAL(swapBytesChanged(bool)));
}
//-----------------------------------------------------------------------------
DialogConvert::~DialogConvert()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxType_currentIndexChanged()
{
    if (this->mWidgetSetup != NULL)
    {
        this->ui->verticalLayout->removeWidget(this->mWidgetSetup);
        delete this->mWidgetSetup;
    }
    QVariant data = this->ui->comboBoxType->itemData(this->ui->comboBoxType->currentIndex());
    this->mConverter->selectConv(data.toString());

    this->ui->checkBoxMirrorBytes->setChecked(this->mConverter->mirror());
    this->ui->checkBoxPack->setChecked(this->mConverter->pack());
    this->ui->checkBoxSwapBytes->setChecked(this->mConverter->swapBytes());
    switch (this->mConverter->length())
    {
    case IConverter::Data8:
        this->ui->comboBoxDataLength->setCurrentIndex(0);
        break;
    case IConverter::Data16:
        this->ui->comboBoxDataLength->setCurrentIndex(1);
        break;
    case IConverter::Data32:
        this->ui->comboBoxDataLength->setCurrentIndex(2);
        break;
    }

    this->mWidgetSetup = this->mConverter->widgetSetup();
    this->ui->verticalLayout->insertWidget(1, this->mWidgetSetup);
    this->mWidgetSetup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->connect(this->mWidgetSetup, SIGNAL(settingsChanged()), SLOT(updatePreview()));
    this->connect(this, SIGNAL(dataLengthChanged(int)), this->mWidgetSetup, SLOT(dataLengthChanged(int)));
    this->connect(this, SIGNAL(dataPackChanged(bool)), this->mWidgetSetup, SLOT(dataPackChanged(bool)));
    this->connect(this, SIGNAL(mirrorBytesChanged(bool)), this->mWidgetSetup, SLOT(mirrorBytesChanged(bool)));
    this->connect(this, SIGNAL(swapBytesChanged(bool)), this->mWidgetSetup, SLOT(swapBytesChanged(bool)));

    //if (this->mConverter->conv().)

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxDataLength_currentIndexChanged()
{
    QVariant data = this->ui->comboBoxDataLength->itemData(this->ui->comboBoxDataLength->currentIndex());
    bool ok;
    int len = data.toInt(&ok);
    emit this->dataLengthChanged(len);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxRotate_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxRotate->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        //BitmapHelper::BitmapHelperTransformCodes rotate = (BitmapHelper::BitmapHelperTransformCodes)a;
        int b = this->mConverter->transform();
        b = b & ~(0x00000003);
        b = b | a;
        this->mConverter->setTransform(b);
    }
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxFlipHorizontal_toggled(bool value)
{
    int a = this->mConverter->transform();
    a = a & ~(BitmapHelper::FlipHorizontal);
    if (value)
        a = a | BitmapHelper::FlipHorizontal;
    this->mConverter->setTransform(a);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxFlipVertical_toggled(bool value)
{
    int a = this->mConverter->transform();
    a = a & ~(BitmapHelper::FlipVertical);
    if (value)
        a = a | BitmapHelper::FlipVertical;
    this->mConverter->setTransform(a);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxInverse_toggled(bool value)
{
    int a = this->mConverter->transform();
    a = a & ~(BitmapHelper::Inverse);
    if (value)
        a = a | BitmapHelper::Inverse;
    this->mConverter->setTransform(a);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxPack_toggled(bool value)
{
    this->ui->radioButtonAlignHigh->setEnabled(!value);
    this->ui->radioButtonAlignLow->setEnabled(!value);
    emit this->dataPackChanged(value);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_radioButtonAlignHigh_toggled(bool value)
{
    if (value)
        this->mConverter->setAlign(IConverter::AlignHigh);
    else
        this->mConverter->setAlign(IConverter::AlignLow);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::updatePreview()
{
    if (this->mData != NULL)
    {
        QString key = this->ui->comboBoxSampleKey->currentText();
        if (!key.isEmpty())
        {
            this->mImageOriginal = QImage(*this->mData->image(key));
            this->ui->labelOriginal->setPixmap(QPixmap::fromImage(this->mImageOriginal));
            QImage processed = this->mConverter->preprocessImage(this->mImageOriginal);
            this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));

            BitmapData data;
            this->mConverter->processImage(processed, &data);

            QString str = this->mConverter->dataToString(data);
            this->ui->plainTextEdit->setPlainText(str);
        }
    }
}
//-----------------------------------------------------------------------------
