#include "picsoutreeitem.h"

PicsouTreeItem::~PicsouTreeItem()
{

}

PicsouTreeItem::PicsouTreeItem(QTreeWidget *view,
                               Type type,
                               const QIcon &icon,
                               const QString &text,
                               QUuid id,
                               int year,
                               int month) :
    QTreeWidgetItem(view, type),
    PicsouItem(id),
    _year(year),
    _month(month)
{
    setIcon(0, icon);
    setText(0, text);
}

PicsouTreeItem::PicsouTreeItem(QTreeWidgetItem *parent,
                               Type type,
                               const QIcon &icon,
                               const QString &text,
                               QUuid id,
                               int year,
                               int month) :
    QTreeWidgetItem(parent, type),
    PicsouItem(id),
    _year(year),
    _month(month)
{
    setIcon(0, icon);
    setText(0, text);
}

PicsouTreeItem *PicsouTreeItem::parent() const
{
    return static_cast<PicsouTreeItem*>(QTreeWidgetItem::parent());
}
