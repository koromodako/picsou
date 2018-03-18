#ifndef PICSOUTREEWIDGETITEM_H
#define PICSOUTREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "ui/picsouitem.h"

class PicsouTreeItem : public QTreeWidgetItem, public PicsouItem
{
public:
    enum Type {
        T_ROOT,
        T_USER,
        T_ACCOUNT,
        T_YEAR,
        T_MONTH
    };

    virtual ~PicsouTreeItem();
    PicsouTreeItem(QTreeWidget *view,
                   Type type,
                   const QIcon &icon,
                   const QString &text,
                   QUuid id,
                   int year=-1,
                   int month=-1);
    PicsouTreeItem(QTreeWidgetItem *parent,
                   Type type,
                   const QIcon &icon,
                   const QString &text,
                   QUuid id,
                   int year=-1,
                   int month=-1);

    PicsouTreeItem *parent() const;

    inline int year() const { return _year; }
    inline int month() const { return _month; }

private:
    int _year;
    int _month;
};

#endif // PICSOUTREEWIDGETITEM_H
