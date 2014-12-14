/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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

#include "dialogsavechanges.h"

#include <QMessageBox>
//-----------------------------------------------------------------------------
DialogSaveChanges::DialogSaveChanges(const QString &documentName, QWidget *parent)
{
    this->mBox = new QMessageBox(parent);
    this->connect(this->mBox, SIGNAL(finished(int)), SLOT(on_messageBox_finished(int)));

    this->mBox->setWindowTitle(tr("Save changes?"));
    this->mBox->setText(tr("Document '%1' have unsaved changes.").arg(documentName));
    this->mBox->setInformativeText(tr("Closing document without saving can cause loss of changes."));
    this->mBox->setIcon(QMessageBox::Question);

    this->mBox->addButton(QMessageBox::Save);
    this->mBox->addButton(QMessageBox::Discard);
    this->mBox->addButton(QMessageBox::Cancel);

    this->mAnswer = Cancel;
}
//-----------------------------------------------------------------------------
DialogSaveChanges::~DialogSaveChanges()
{
    delete this->mBox;
}
//-----------------------------------------------------------------------------
int DialogSaveChanges::answer()
{
    return this->mAnswer;
}
//-----------------------------------------------------------------------------
int DialogSaveChanges::exec()
{
    return this->mBox->exec();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_messageBox_finished(int result)
{
    switch (result)
    {
        case QMessageBox::Save:
        {
            this->mAnswer = Save;
            break;
        }
        case QMessageBox::Discard:
        {
            this->mAnswer = DontSave;
            break;
        }
        case QMessageBox::Cancel:
        {
            this->mAnswer = Cancel;
            break;
        }
        default:
        {
            this->mAnswer = Cancel;
            break;
        }
    }
}
//-----------------------------------------------------------------------------
