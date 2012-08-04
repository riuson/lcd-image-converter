#ifndef CONVERTERHELPER_H
#define CONVERTERHELPER_H
//-----------------------------------------------------------------------------
class QStringList;
//-----------------------------------------------------------------------------
class ConverterHelper
{
public:
    static void packDataPreview(QStringList *list, QStringList &colors, int bits, bool pack, bool alignToHigh);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERHELPER_H
