#ifndef PICSOULISTITEM_H
#define PICSOULISTITEM_H

#include <QListWidgetItem>

#include "ui/picsouitem.h"

class PicsouListItem : public QListWidgetItem, public PicsouItem
{
public:
    virtual ~PicsouListItem();
    PicsouListItem(const QString &text, QListWidget *view, QUuid id);
};

#endif // PICSOULISTITEM_H
