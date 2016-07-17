#ifndef TFONTPARAMETERS
#define TFONTPARAMETERS

#include <QObject>
//-----------------------------------------------------------------------------
struct tFontParameters
{
    QString family;
    QString style;
    int size;
    bool monospaced;
    bool antiAliasing;
    int ascent;
    int descent;
};
//-----------------------------------------------------------------------------
#endif // TFONTPARAMETERS

