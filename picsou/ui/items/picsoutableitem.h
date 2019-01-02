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
#ifndef PICSOUTABLEITEM_H
#define PICSOUTABLEITEM_H

#include <QDate>
#include <QTableWidgetItem>

#include "ui/picsouitem.h"

class PicsouTableItem : public QTableWidgetItem, public PicsouItem
{
public:
    enum Type {
        NORMAL,
        SCHEDULED,
        CHECKABLE
    };

    PicsouTableItem(const QIcon &icon,
                    const QDate &date,
                    QUuid id,
                    Type type);
};

#endif // PICSOUTABLEITEM_H
