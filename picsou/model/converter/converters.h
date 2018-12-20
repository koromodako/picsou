#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "app/picsouuiservice.h"
#include <QJsonDocument>

typedef bool (*db_converter_t)(QJsonDocument *doc, PicsouUIServicePtr ui_svc);

bool convert_100_110(QJsonDocument *doc, PicsouUIServicePtr ui_svc);
bool convert_110_200(QJsonDocument *doc, PicsouUIServicePtr ui_svc);

#endif // CONVERTERS_H
