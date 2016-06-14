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

OperationArea FontMinimizeHeight::area() const
{
    return OperationArea::Document;
}

void FontMinimizeHeight::apply(IDocument *doc, const QString &itemKey)
{
    Q_UNUSED(itemKey)

    int left = std::numeric_limits<int>::max();
    int top = std::numeric_limits<int>::max();
    int right = 0;
    int bottom = 0;
    int l, t, r, b;

    // find limits
    QStringList keys = doc->dataContainer()->keys();
    QListIterator<QString> it(keys);
    it.toFront();
    while (it.hasNext())
    {
        QString key = it.next();
        const QImage *original = doc->dataContainer()->image(key);

        BitmapHelper::findEmptyArea(original, &l, &t, &r, &b);

        left = qMin(left, l);
        top = qMin(top, t);
        right = qMin(right, r);
        bottom = qMin(bottom, b);
    }

    DialogCanvasResize dialog(doc->dataContainer(), this->mParentWidget);
    dialog.selectKeys(keys);
    dialog.setResizeInfo(-left, -top, -right, -bottom);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.resizeInfo(&left, &top, &right, &bottom);

        if (left != 0 || top != 0 || right != 0 || bottom != 0)
        {
            doc->beginChanges();

            QStringListIterator iterator(keys);
            while (iterator.hasNext())
            {
                QString key = iterator.next();

                const QImage *original = doc->dataContainer()->image(key);
                QImage result = BitmapHelper::crop(original, left, top, right, bottom, BitmapHelper::detectBackgroundColor(original));
                doc->dataContainer()->setImage(key, &result);
            }

            doc->endChanges(false);
        }
    }
}

}
