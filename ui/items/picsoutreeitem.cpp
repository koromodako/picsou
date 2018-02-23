#include "picsoutreeitem.h"

PicsouTreeItem::~PicsouTreeItem()
{

}

PicsouTreeItem::PicsouTreeItem(QTreeWidget *view,
                               Type type,
                               const QString &text,
                               QUuid id) :
    QTreeWidgetItem(view, type),
    PicsouItem(id)
{
    setText(0, text);
}

PicsouTreeItem::PicsouTreeItem(QTreeWidgetItem *parent,
                               Type type,
                               const QString &text,
                               QUuid id) :
    QTreeWidgetItem(parent, type),
    PicsouItem(id)
{
    setText(0, text);
}
