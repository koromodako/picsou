#ifndef CONVERTER_H
#define CONVERTER_H

#include "semver.h"
#include <QJsonDocument>

class Converter
{
public:
    virtual ~Converter();
    static bool convert(QJsonDocument *doc, SemVer db_version);

private:
    Converter();
};

#endif // CONVERTER_H
