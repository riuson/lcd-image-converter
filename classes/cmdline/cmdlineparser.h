#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class CmdLineParser : public QObject
{
    Q_OBJECT
public:
    explicit CmdLineParser(QObject *parent = 0);

    bool hasArguments() const;
    int process();

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // CMDLINEPARSER_H
