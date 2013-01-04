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

#include "setuptabreordering.h"
#include "ui_setuptabreordering.h"
//-----------------------------------------------------------------------------
#include "reorderingpreviewmodel.h"
#include "matrixitemdelegate.h"
#include "preset.h"
#include "prepareoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
SetupTabReordering::SetupTabReordering(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabReordering)
{
    ui->setupUi(this);
    this->mPreset = preset;
    this->mMenu = NULL;

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->mReorderingModel = new ReorderingPreviewModel(this->mPreset, this);
    this->ui->tableViewOperations->setModel(this->mReorderingModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();

    this->ui->tableViewOperations->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this->ui->tableViewOperations->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_tableViewOperations_customContextMenuRequested(QPoint)));

    this->mMatrixItemDelegate = new MatrixItemDelegate(this);
    this->ui->tableViewOperations->setItemDelegate(this->mMatrixItemDelegate);
}
//-----------------------------------------------------------------------------
SetupTabReordering::~SetupTabReordering()
{
    if (this->mMenu != NULL)
        delete this->mMenu;

    delete this->mMatrixItemDelegate;
    delete this->mReorderingModel;

    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabReordering::matrixChanged()
{
    int index = this->ui->comboBoxConversionType->findData(this->mPreset->prepare()->convType());
    if (index >= 0)
        this->ui->comboBoxConversionType->setCurrentIndex(index);

    this->mReorderingModel->callReset();
    this->ui->tableViewOperations->setModel(NULL);
    this->ui->tableViewOperations->setModel(this->mReorderingModel);
    this->ui->tableViewOperations->update();
    this->ui->tableViewOperations->resizeRowsToContents();
    this->ui->tableViewOperations->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
void SetupTabReordering::on_comboBoxConversionType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxConversionType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mPreset->prepare()->setConvType((ConversionType)a);
    }
}
//-----------------------------------------------------------------------------
void SetupTabReordering::on_tableViewOperations_customContextMenuRequested(const QPoint &point)
{
    QModelIndex index = this->ui->tableViewOperations->indexAt(point);
    QItemSelectionModel *selection = this->ui->tableViewOperations->selectionModel();

    if (this->mMenu != NULL)
    {
        delete this->mMenu;
        this->mMenu = NULL;
    }

    if (index.isValid())
    {
        ReorderingPreviewModel::RowType type = this->mReorderingModel->rowType(index.row());
        QModelIndexList list = selection->selectedIndexes();

        switch (type)
        {
        case ReorderingPreviewModel::Source:
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
                    QAction *action = left->addAction(QString("<< %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(-i));

                    action = right->addAction(QString(">> %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(i));
                }

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            }
            break;
        }
        case ReorderingPreviewModel::Operation:
        {
                this->mMenu = new QMenu(tr("Operation"), this);

                int operationIndex = index.row() - 1;

                quint32 mask;
                int shift;
                bool left;

                this->mPreset->reordering()->operation(operationIndex, &mask, &shift, &left);

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
        default:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
void SetupTabReordering::operationAdd()
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
        this->mPreset->reordering()->operationAdd(mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void SetupTabReordering::operationShift()
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
        this->mPreset->reordering()->operation(index, &mask, &shift, &left);

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

        this->mPreset->reordering()->operationReplace(index, mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void SetupTabReordering::operationRemove()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 index = var.toUInt(&ok);

    if (ok)
    {
        this->mPreset->reordering()->operationRemove(index);
    }
}
//-----------------------------------------------------------------------------
