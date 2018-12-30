#ifndef PICSOUCOMMANDLINEPARSER_H
#define PICSOUCOMMANDLINEPARSER_H

#include <QCoreApplication>
#include <QCommandLineParser>

class PicsouCommandLineParser : public QCommandLineParser
{
    Q_DECLARE_TR_FUNCTIONS(PicsouCommandLineParser)

public:
    PicsouCommandLineParser();
};

#endif // PICSOUCOMMANDLINEPARSER_H
