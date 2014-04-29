#ifndef IPARSERMODE_H
#define IPARSERMODE_H
//-----------------------------------------------------------------------------
class QCommandLineParser;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class IParserMode
{
public:
    virtual QString modeName() const = 0;
    virtual void fillParser(QCommandLineParser *parser) const = 0;
    virtual void collectArguments(QCommandLineParser *parser) = 0;
    virtual int process() = 0;
};
Q_DECLARE_INTERFACE (IParserMode,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // IPARSERMODE_H
