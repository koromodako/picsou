#include "picsoucommandlineparser.h"
#include "utils/macro.h"

PicsouCommandLineParser::PicsouCommandLineParser() :
    QCommandLineParser()
{
    setApplicationDescription(tr("Keep in touch with your expenses."));
    addHelpOption();
    addVersionOption();
    addPositionalArgument("database", tr("Database file."));
}
