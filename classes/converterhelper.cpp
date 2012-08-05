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

#include "converterhelper.h"
//-----------------------------------------------------------------------------
#include <QStringList>
//-----------------------------------------------------------------------------
void ConverterHelper::packDataPreview(QStringList *list, QStringList &colors, int bits, bool pack, bool alignToHigh)
{
    QStringList temp;
    for (int i = 0, j = 0, k = 0; i < 80; i++)
    {
        QString a = colors.at(k++);
        temp.append(a.replace(".", QString("%1.").arg(j)));
        if (k == colors.length())// end of point (color bits)
        {
            j++;
            k = 0;
            if (!pack)
            {
                if ((i % bits) + colors.length() > bits - 1)
                {
                    while (i % bits != bits - 1)
                    {
                        if (alignToHigh)
                            temp.append("0");
                        else
                            temp.prepend("0");
                        i++;
                    }
                    list->append(temp);
                    temp.clear();
                }
            }
            else
            {
                list->append(temp);
                temp.clear();
            }
        }
    }
    list->append(temp);
    temp.clear();
}
//-----------------------------------------------------------------------------
