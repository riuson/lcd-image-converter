#ifndef IEDIT_H
#define IEDIT_H
//-----------------------------------------------------------------------------
#include <QObject>

class QString;
//-----------------------------------------------------------------------------
class IEdit
{
public:
    virtual void inverse() = 0;
    virtual void save(const QString &filename) = 0;
    virtual void load(const QString &filename) = 0;
};
//-----------------------------------------------------------------------------
#endif // IEDIT_H
