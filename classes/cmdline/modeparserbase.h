#ifndef MODEPARSERBASE_H
#define MODEPARSERBASE_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QCommandLineParser;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class ModeParserBase : public QObject
{
    Q_OBJECT
public:
    explicit ModeParserBase(QCommandLineParser *parser, QObject *parent = 0);

    virtual void fillParser() const = 0;
    virtual bool collectArguments() = 0;
    virtual int process() = 0;

protected:
    QCommandLineParser *mParser;
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // MODEPARSERBASE_H
