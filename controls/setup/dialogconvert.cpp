/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "dialogconvert.h"
#include "ui_dialogconvert.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QSettings>
#include <QStringList>
#include <QInputDialog>
#include <QMenu>
#include "idatacontainer.h"
#include "converterhelper.h"
#include "dialogpreview.h"
#include "matrixpreviewmodel.h"
#include "conversionmatrixoptions.h"
#include "conversionmatrix.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);
    this->mPreview = NULL;
    this->mMenu = NULL;

    this->mData = dataContainer;
    this->mMatrix = new ConversionMatrix(this);

    this->mMatrixModel = new MatrixPreviewModel(this->mMatrix, this);
    this->ui->tableViewOperations->setModel(this->mMatrixModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(tr("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxBlockSize->addItem(tr("8 bit"), Data8);
    this->ui->comboBoxBlockSize->addItem(tr("16 bit"), Data16);
    this->ui->comboBoxBlockSize->addItem(tr("24 bit"), Data24);
    this->ui->comboBoxBlockSize->addItem(tr("32 bit"), Data32);

    this->ui->comboBoxRotate->addItem(tr("None"), QVariant(RotateNone));
    this->ui->comboBoxRotate->addItem(tr("90\u00b0 Clockwise"), QVariant(Rotate90));
    this->ui->comboBoxRotate->addItem(tr("180\u00b0"), QVariant(Rotate180));
    this->ui->comboBoxRotate->addItem(tr("90\u00b0 Counter-Clockwise"), QVariant(Rotate270));

    if (this->mMatrix->options()->bytesOrder() == BytesOrderLittleEndian)
        this->ui->radioButtonLittleEndian->setChecked(true);
    else
        this->ui->radioButtonBigEndian->setChecked(true);

    QSettings sett;
    sett.beginGroup("presets");
    QString selectedPreset = sett.value("selected", QVariant("")).toString();
    sett.endGroup();

    this->connect(this->mMatrix, SIGNAL(changed()), SLOT(updatePreview()));

    this->fillPresetsList();

    int presetIndex = this->ui->comboBoxPresets->findText(selectedPreset);
    if (presetIndex >= 0)
        this->ui->comboBoxPresets->setCurrentIndex(presetIndex);
}
//-----------------------------------------------------------------------------
DialogConvert::~DialogConvert()
{
    if (this->result() == QDialog::Accepted)
    {
        QSettings sett;
        sett.beginGroup("presets");
        sett.setValue("selected", this->ui->comboBoxPresets->currentText());
        sett.endGroup();
    }

    if (this->mMenu != NULL)
        delete this->mMenu;

    if (this->mPreview != NULL)
        delete this->mPreview;

    delete ui;
    delete this->mMatrixModel;
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
void DialogConvert::fillPresetsList()
{
    QString current = this->ui->comboBoxPresets->currentText();

    this->ui->comboBoxPresets->clear();

    QSettings sett;
    sett.beginGroup("presets");
    QStringList names = sett.childGroups();
    sett.endGroup();

    this->ui->comboBoxPresets->addItems(names);

    if (names.contains(current))
    {
        this->ui->comboBoxPresets->setCurrentIndex(names.indexOf(current));
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::presetLoad(const QString &name)
{
    if (this->mMatrix->load(name))
    {
        // update gui

        this->ui->tableViewOperations->setModel(NULL);
        this->ui->tableViewOperations->setModel(this->mMatrixModel);
        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();

        int index = this->ui->comboBoxConversionType->findData(this->mMatrix->options()->convType());
        if (index >= 0)
            this->ui->comboBoxConversionType->setCurrentIndex(index);

        index = this->ui->comboBoxMonochromeType->findData(this->mMatrix->options()->monoType());
        if (index >= 0)
            this->ui->comboBoxMonochromeType->setCurrentIndex(index);

        index = this->ui->comboBoxBlockSize->findData(this->mMatrix->options()->blockSize());
        if (index >= 0)
            this->ui->comboBoxBlockSize->setCurrentIndex(index);

        index = this->ui->comboBoxRotate->findData(this->mMatrix->options()->rotate());
        if (index >= 0)
            this->ui->comboBoxRotate->setCurrentIndex(index);

        this->ui->horizontalScrollBarEdge->setValue(this->mMatrix->options()->edge());

        this->ui->checkBoxFlipHorizontal->setChecked(this->mMatrix->options()->flipHorizontal());
        this->ui->checkBoxFlipVertical->setChecked(this->mMatrix->options()->flipVertical());
        this->ui->checkBoxInverse->setChecked(this->mMatrix->options()->inverse());

        if (this->mMatrix->options()->bytesOrder() == BytesOrderLittleEndian)
            this->ui->radioButtonLittleEndian->setChecked(true);
        else
            this->ui->radioButtonBigEndian->setChecked(true);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::presetSaveAs(const QString &name)
{
    this->mMatrix->save(name);
    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::presetRemove(const QString &name)
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    sett.endGroup();

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::updatePreview()
{
    if (this->mData != NULL)
    {
        this->mMatrixModel->callReset();
        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();
        this->ui->tableViewOperations->resizeRowsToContents();

        if (this->mPreview != NULL)
            this->mPreview->updatePreview();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPreview_clicked()
{
    if (this->mPreview == NULL)
    {
        this->mPreview = new DialogPreview(this->mData, this->mMatrix, this);
        QObject::connect(this->mPreview, SIGNAL(accepted()), this, SLOT(previewClosed()));
        QObject::connect(this->mPreview, SIGNAL(rejected()), this, SLOT(previewClosed()));
    }

    this->mPreview->show();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_radioButtonLittleEndian_toggled(bool value)
{
    if (value)
        this->mMatrix->options()->setBytesOrder(BytesOrderLittleEndian);
    else
        this->mMatrix->options()->setBytesOrder(BytesOrderBigEndian);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxConversionType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxConversionType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setConvType((ConversionType)a);

        if (this->mMatrix->options()->convType() == ConversionTypeMonochrome)
        {
            this->ui->comboBoxMonochromeType->setEnabled(true);
            if (this->mMatrix->options()->monoType() == MonochromeTypeEdge)
                this->ui->horizontalScrollBarEdge->setEnabled(true);
            else
                this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
        else
        {
            this->ui->comboBoxMonochromeType->setEnabled(false);
            this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxMonochromeType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxMonochromeType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setMonoType((MonochromeType)a);

        if (this->mMatrix->options()->convType() == ConversionTypeMonochrome)
        {
            this->ui->comboBoxMonochromeType->setEnabled(true);
            if (this->mMatrix->options()->monoType() == MonochromeTypeEdge)
                this->ui->horizontalScrollBarEdge->setEnabled(true);
            else
                this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
        else
        {
            this->ui->comboBoxMonochromeType->setEnabled(false);
            this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxBlockSize_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxBlockSize->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setBlockSize((DataBlockSize)a);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxRotate_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxRotate->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        Rotate rotate = (Rotate)a;
        this->mMatrix->options()->setRotate(rotate);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxFlipHorizontal_toggled(bool value)
{
    this->mMatrix->options()->setFlipHorizontal(value);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxFlipVertical_toggled(bool value)
{
    this->mMatrix->options()->setFlipVertical(value);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_checkBoxInverse_toggled(bool value)
{
    this->mMatrix->options()->setInverse(value);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPresetSaveAs_clicked()
{
    QSettings sett;
    sett.beginGroup("presets");

    QStringList names = sett.childGroups();

    sett.endGroup();

    QInputDialog dialog(this);
    dialog.setComboBoxItems(names);
    dialog.setComboBoxEditable(true);

    QString current = this->ui->comboBoxPresets->currentText();
    bool ok;

    QString result = dialog.getItem(this, tr("Enter preset name"), tr("Preset name:"), names, names.indexOf(current), true, &ok);
    if (ok && !result.isEmpty())
    {
        this->presetSaveAs(result);
    }

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPresetRemove_clicked()
{
    QString name = this->ui->comboBoxPresets->currentText();
    this->presetRemove(name);

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxPresets_currentIndexChanged(int index)
{
    QString name = this->ui->comboBoxPresets->itemText(index);
    this->presetLoad(name);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_horizontalScrollBarEdge_valueChanged(int value)
{
    this->mMatrix->options()->setEdge(value);
}
//-----------------------------------------------------------------------------
void DialogConvert::on_tableViewOperations_customContextMenuRequested(const QPoint &point)
{
    QModelIndex index = this->ui->tableViewOperations->indexAt(point);
    QItemSelectionModel *selection = this->ui->tableViewOperations->selectionModel();

    if (this->mMenu != NULL)
        delete this->mMenu;

    if (index.isValid())
    {
        MatrixPreviewModel::RowType type = this->mMatrixModel->rowType(index.row());
        QModelIndexList list = selection->selectedIndexes();

        switch (type)
        {
        case MatrixPreviewModel::Source:
        {
            bool found = false;
            for (int i = 0; i < list.length() && !found; i++)
            {
                if (list.at(i).row() == 0)
                    found = true;
            }
            if (found)
            {
                this->mMenu = new QMenu(tr("Source"), this);

                QMenu *left = new QMenu(tr("Add \"Left Shift\""), this->mMenu);
                QMenu *right = new QMenu(tr("Add \"Right Shift\""), this->mMenu);

                this->mMenu->addMenu(left);
                this->mMenu->addMenu(right);

                for (int i = 0; i < 32; i++)
                {
                    QAction *action = left->addAction(tr("<< %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(-i));

                    action = right->addAction(tr(">> %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(i));
                }

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            }
            break;
        }
        case MatrixPreviewModel::Operation:
        {
                this->mMenu = new QMenu(tr("Operation"), this);

                int operationIndex = index.row() - 1;

                quint32 mask;
                int shift;
                bool left;

                this->mMatrix->operation(operationIndex, &mask, &shift, &left);

                QAction *actionLeft = this->mMenu->addAction(tr("Shift left"), this, SLOT(operationShift()));
                QAction *actionRight = this->mMenu->addAction(tr("Shift right"), this, SLOT(operationShift()));
                QAction *actionRemove = this->mMenu->addAction(tr("Remove"), this, SLOT(operationRemove()));
                Q_UNUSED(actionRemove)

                quint32 data = operationIndex;

                actionLeft->setData(QVariant(data | 0x80000000));
                actionRight->setData(QVariant(data));
                actionRemove->setData(QVariant(data));

                if (shift >= 31)
                {
                    if (left)
                        actionLeft->setEnabled(false);
                    else
                        actionRight->setEnabled(false);
                }

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            break;
        }
        case MatrixPreviewModel::MaskUsed:
        case MatrixPreviewModel::MaskAnd:
        case MatrixPreviewModel::MaskOr:
        {
                this->mMenu = new QMenu(tr("Mask"), this);

                QAction *actionSet = this->mMenu->addAction(tr("Set all 1"), this, SLOT(maskReset()));
                QAction *actionReset = this->mMenu->addAction(tr("Set all 0"), this, SLOT(maskReset()));

                quint32 data = (quint32)type;

                actionSet->setData(QVariant(data | 0x80000000));
                actionReset->setData(QVariant(data));

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            break;
        }
        case MatrixPreviewModel::Result:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::previewClosed()
{
    if (this->mPreview != NULL)
    {
        delete this->mPreview;
        this->mPreview = NULL;
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::operationAdd()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    int shift = var.toInt(&ok);

    if (ok)
    {
        QItemSelectionModel *selection = this->ui->tableViewOperations->selectionModel();
        QModelIndexList list = selection->selectedIndexes();

        bool left = shift < 0;
        shift = qAbs(shift);

        quint32 mask = 0;
        for (int i = 0; i < list.length(); i++)
        {
            if (list.at(i).row() == 0)
            {
                mask |= 0x00000001 << (31 - list.at(i).column());
            }
        }
        this->mMatrix->operationAdd(mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::operationShift()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 index = var.toUInt(&ok);

    if (ok)
    {
        bool leftShift = ((index & 0x80000000) != 0);
        index &= 0x7fffffff;

        quint32 mask;
        int shift;
        bool left;
        this->mMatrix->operation(index, &mask, &shift, &left);

        if (leftShift)
        {
            if (shift > 0)
            {
                if (left)
                    shift++;
                else
                    shift--;
            }
            else
            {
                shift = 1;
                left = true;
            }
        }
        else
        {
            if (shift > 0)
            {
                if (left)
                    shift--;
                else
                    shift++;
            }
            else
            {
                shift = 1;
                left = false;
            }
        }

        this->mMatrix->operationReplace(index, mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::operationRemove()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 index = var.toUInt(&ok);

    if (ok)
    {
        this->mMatrix->operationRemove(index);
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::maskReset()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 i = var.toUInt(&ok);

    if (ok)
    {
        MatrixPreviewModel::RowType type = (MatrixPreviewModel::RowType) (i & 0x7fffffff);

        quint32 mask = 0;
        if ((i & 0x80000000) != 0)
            mask = 0xffffffff;

        switch (type)
        {
        case MatrixPreviewModel::MaskUsed:
        {
            this->mMatrix->options()->setMaskUsed(mask);
            break;
        }
        case MatrixPreviewModel::MaskAnd:
        {
            this->mMatrix->options()->setMaskAnd(mask);
            break;
        }
        case MatrixPreviewModel::MaskOr:
        {
            this->mMatrix->options()->setMaskOr(mask);
            break;
        }
        case MatrixPreviewModel::Source:
        case MatrixPreviewModel::Operation:
        case MatrixPreviewModel::Result:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
