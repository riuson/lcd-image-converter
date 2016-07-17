#ifndef TFONTPARAMETERS
#define TFONTPARAMETERS

#include <QObject>
#include <QColor>
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
    QColor foreground;
    QColor background;
};
//-----------------------------------------------------------------------------
#endif // TFONTPARAMETERS

