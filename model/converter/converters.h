#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QJsonDocument>

class PicsouUIService;

typedef bool (*db_converter_t)(QJsonDocument *doc, PicsouUIService *ui_svc);

bool convert_100_110(QJsonDocument *doc, PicsouUIService *ui_svc);
bool convert_110_200(QJsonDocument *doc, PicsouUIService *ui_svc);

#endif // CONVERTERS_H
