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

#include "documentoperator.h"
#include "idocument.h"
#include "ioperation.h"
#include "datacontainer.h"

namespace Operations {

DocumentOperator::DocumentOperator(QObject *parent)
    : QObject(parent)
{
}

void DocumentOperator::setKeys(const QStringList keys)
{
    this->mSelectedKeys = keys;
}

void DocumentOperator::apply(IDocument *doc, IOperation &operation)
{
    if (operation.prepare(doc))
    {
        doc->beginChanges();

        operation.applyDocument(doc);

        QStringList keys = this->mSelectedKeys.length() == 0 ? doc->dataContainer()->keys() : this->mSelectedKeys;
        QListIterator<QString> it(keys);
        it.toFront();

        while (it.hasNext())
        {
            QString key = it.next();
            operation.applyItem(doc, key);
        }

        doc->endChanges(false);
    }
}

}
