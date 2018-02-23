#include "picsoulistitem.h"

PicsouListItem::~PicsouListItem()
{

}

PicsouListItem::PicsouListItem(const QString &text, QListWidget *view, QUuid id) :
    QListWidgetItem(text, view),
    PicsouItem(id)
{

}
