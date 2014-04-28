#ifndef CMDLINE_H
#define CMDLINE_H
//-----------------------------------------------------------------------------
class QString;
class QStringList;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class CmdLineParser;
//-----------------------------------------------------------------------------
class CmdLine
{
public:
    CmdLine(const QStringList &arguments);
    ~CmdLine();

    bool needProcess() const;
    int process();
private:
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
