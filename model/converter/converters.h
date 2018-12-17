#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QJsonDocument>

class PicsouUIService;

typedef void (*db_converter_t)(QJsonDocument *doc, PicsouUIService *ui_svc);

void convert_100_110(QJsonDocument *doc, PicsouUIService *ui_svc);
void convert_110_200(QJsonDocument *doc, PicsouUIService *ui_svc);

#endif // CONVERTERS_H
