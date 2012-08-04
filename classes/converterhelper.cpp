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
