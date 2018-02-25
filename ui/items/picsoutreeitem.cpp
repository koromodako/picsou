#include "picsoutreeitem.h"

PicsouTreeItem::~PicsouTreeItem()
{

}

PicsouTreeItem::PicsouTreeItem(QTreeWidget *view,
                               Type type,
                               const QIcon &icon,
                               const QString &text,
                               QUuid id) :
    QTreeWidgetItem(view, type),
    PicsouItem(id)
{
    setIcon(0, icon);
    setText(0, text);
}

PicsouTreeItem::PicsouTreeItem(QTreeWidgetItem *parent,
                               Type type,
                               const QIcon &icon,
                               const QString &text,
                               QUuid id) :
    QTreeWidgetItem(parent, type),
    PicsouItem(id)
{
    setIcon(0, icon);
    setText(0, text);
}

PicsouTreeItem *PicsouTreeItem::parent() const
{
    return static_cast<PicsouTreeItem*>(QTreeWidgetItem::parent());
}
