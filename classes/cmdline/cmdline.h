#ifndef CMDLINE_H
#define CMDLINE_H
//-----------------------------------------------------------------------------
class QCoreApplication;
class QString;
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

    int convertBinaryImage(
            const QString &inputFilename,
            const QString &outputFilename,
            const QString &templateFilename,
            const QString &presetName,
            const QString &documentName);
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CMDLINE_H
