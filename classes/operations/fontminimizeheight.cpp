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

#include "fontminimizeheight.h"
#include "dialogcanvasresize.h"
#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace Operations
{

FontMinimizeHeight::FontMinimizeHeight(QWidget *parentWidget, QObject *parent)
    : QObject(parent)
{
    this->mParentWidget = parentWidget;
}

bool FontMinimizeHeight::prepare(const IDocument *doc)
{
    this->mLeft = std::numeric_limits<int>::max();
    this->mTop = std::numeric_limits<int>::max();
    this->mRight = 0;
    this->mBottom = 0;

    // find limits
    QStringList keys = doc->dataContainer()->keys();
    QListIterator<QString> it(keys);
    it.toFront();

    while (it.hasNext())
    {
        QString key = it.next();
        const QImage *original = doc->dataContainer()->image(key);

        int l, t, r, b;
        BitmapHelper::findEmptyArea(original, &l, &t, &r, &b);

        this->mLeft = qMin(this->mLeft, l);
        this->mTop = qMin(this->mTop, t);
        this->mRight = qMin(this->mRight, r);
        this->mBottom = qMin(this->mBottom, b);
    }

    DialogCanvasResize dialog(doc->dataContainer(), this->mParentWidget);
    dialog.selectKeys(keys);
    dialog.setResizeInfo(-this->mLeft, -this->mTop, -this->mRight, -this->mBottom);

    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.resizeInfo(&this->mLeft, &this->mTop, &this->mRight, &this->mBottom);

        if (this->mLeft != 0 || this->mTop != 0 || this->mRight != 0 || this->mBottom != 0)
        {
            return true;
        }
    }

    return false;
}

void FontMinimizeHeight::applyDocument(IDocument *doc)
{
    Q_UNUSED(doc)
}

void FontMinimizeHeight::applyItem(IDocument *doc, const QString &itemKey)
{
    const QImage *original = doc->dataContainer()->image(itemKey);
    QImage result = BitmapHelper::crop(original, this->mLeft, this->mTop, this->mRight, this->mBottom, BitmapHelper::detectBackgroundColor(original));
    doc->dataContainer()->setImage(itemKey, &result);
}

}
