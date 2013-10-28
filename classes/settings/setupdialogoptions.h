#ifndef SETUPDIALOGOPTIONS_H
#define SETUPDIALOGOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class SetupDialogOptions : public QObject
{
    Q_OBJECT
public:
    static int animationTotalTime();
    static void setAnimationTime(int value);
    static int animationInterval();
    static void setAnimationInterval(int value);
};
//-----------------------------------------------------------------------------
#endif // SETUPDIALOGOPTIONS_H
