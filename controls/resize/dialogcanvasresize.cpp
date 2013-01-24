/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "dialogcanvasresize.h"
#include "ui_dialogcanvasresize.h"

#include <QItemSelectionModel>
#include "resizemodel.h"
//-----------------------------------------------------------------------------
DialogCanvasResize::DialogCanvasResize(DataContainer *container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCanvasResize)
{
    ui->setupUi(this);

    this->mContainer = container;
    this->mModel = new ResizeModel(container, this);
    this->ui->tableView->setModel(this->mModel);

    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    this->connect(this->ui->spinBoxLeft,   SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxTop,    SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxRight,  SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxBottom, SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
}
//-----------------------------------------------------------------------------
DialogCanvasResize::~DialogCanvasResize()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::updatePreview()
{
    /*QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndex index = this->mModel->index(selectionModel->currentIndex().row(), selectionModel->currentIndex().column());

        QVariant value = this->mModel->data(index, ResizeModel::CropResultRole);
        QImage image = value.value<QImage>();
        QPixmap pixmap = QPixmap::fromImage(image);
        this->ui->labelPreview->setPixmap(pixmap);
    }*/
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::spinBox_valueChanged(int value)
{
    Q_UNUSED(value);

    this->mModel->setCrop(
                this->ui->spinBoxLeft->value(),
                this->ui->spinBoxTop->value(),
                this->ui->spinBoxRight->value(),
                this->ui->spinBoxBottom->value());

    this->ui->tableView->resizeRowsToContents();
    this->ui->tableView->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::on_spinBoxScale_valueChanged(int value)
{
    this->mModel->setScale(value);

    this->ui->tableView->resizeRowsToContents();
    this->ui->tableView->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::on_pushButtonReset_clicked()
{
    this->ui->spinBoxLeft->setValue(0);
    this->ui->spinBoxTop->setValue(0);
    this->ui->spinBoxRight->setValue(0);
    this->ui->spinBoxBottom->setValue(0);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
