#include "cmdline.h"
#include "cmdlineparser.h"
#include <QDebug>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
CmdLine::CmdLine(const QCoreApplication *app)
{
    this->mApp = app;
    this->mParser = new CmdLineParser(this->mApp);
}
//-----------------------------------------------------------------------------
CmdLine::~CmdLine()
{
    delete this->mParser;
}
//-----------------------------------------------------------------------------
bool CmdLine::needProcess() const
{
    if (this->mParser->mode() == ModeConvert)
    {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
int CmdLine::process()
{
    if (this->mParser->mode() == ModeConvert)
    {
        if (this->mParser->documentType() == DocumentTypeImage)
        {
            return 0;
        }
    }

    return 1;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
