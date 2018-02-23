#ifndef PICSOUTABLEITEM_H
#define PICSOUTABLEITEM_H

#include <QTableWidgetItem>

#include "ui/picsouitem.h"

class PicsouTableItem : public QTableWidgetItem, public PicsouItem
{
public:
    virtual ~PicsouTableItem();
    PicsouTableItem(const QIcon &icon,
                    const QString &text,
                    QUuid id);
};

#endif // PICSOUTABLEITEM_H
