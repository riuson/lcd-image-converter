#ifndef CMDLINE_H
#define CMDLINE_H
//-----------------------------------------------------------------------------
class QCoreApplication;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class CmdLineParser;
//-----------------------------------------------------------------------------
class CmdLine
{
public:
    CmdLine(const QCoreApplication *app);
    ~CmdLine();

    bool needProcess() const;
    int process();
private:
    const QCoreApplication *mApp;
    CmdLineParser *mParser;
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CMDLINE_H
