#include "picsoutableitem.h"

PicsouTableItem::~PicsouTableItem()
{

}

PicsouTableItem::PicsouTableItem(const QIcon &icon,
                                 const QString &text,
                                 QUuid id) :
    QTableWidgetItem(icon, text),
    PicsouItem(id)
{

}
