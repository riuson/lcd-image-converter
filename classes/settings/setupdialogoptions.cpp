#include "setupdialogoptions.h"

#include <QSettings>
#include <QVariant>
//-----------------------------------------------------------------------------
int SetupDialogOptions::animationTotalTime()
{
    QSettings sett;
    sett.beginGroup("setup-dialog");
    bool ok;
    int result = sett.value("animationTotalTime", QVariant(60)).toInt(&ok);
    sett.endGroup();

    if (!ok)
        result = 60;

    return result;
}
//-----------------------------------------------------------------------------
void SetupDialogOptions::setAnimationTime(int value)
{
    QSettings sett;
    sett.beginGroup("setup-dialog");
    sett.setValue("animationTotalTime", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
int SetupDialogOptions::animationInterval()
{
    QSettings sett;
    sett.beginGroup("setup-dialog");
    bool ok;
    int result = sett.value("animationInterval", QVariant(100)).toInt(&ok);
    sett.endGroup();

    if (!ok)
        result = 100;

    return result;
}
//-----------------------------------------------------------------------------
void SetupDialogOptions::setAnimationInterval(int value)
{
    QSettings sett;
    sett.beginGroup("setup-dialog");
    sett.setValue("animationInterval", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
