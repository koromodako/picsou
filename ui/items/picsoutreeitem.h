/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
                   int month=-1,
                   bool wrapped=false);
    PicsouTreeItem(QTreeWidgetItem *parent,
                   Type type,
                   const QIcon &icon,
                   const QString &text,
                   QUuid id,
                   int year=-1,
                   int month=-1,
                   bool wrapped=false);

    PicsouTreeItem *parent() const;

    inline int year() const { return m_year; }
    inline int month() const { return m_month; }
    inline bool wrapped() const { return m_wrapped; }

private:
    int m_year;
    int m_month;
    bool m_wrapped;
};

#endif // PICSOUTREEWIDGETITEM_H
