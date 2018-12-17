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
#ifndef PICSOUUIVIEWER_H
#define PICSOUUIVIEWER_H

#include <QUuid>
#include <QWidget>
#include "picsouui.h"
#include "model/object/picsoudb.h"

class PicsouModelService;

class PicsouUIViewer : public QWidget, public PicsouUI
{
public:
    virtual ~PicsouUIViewer();

public slots:
    virtual void refresh(const PicsouDBPtr db)=0;

protected:
    PicsouUIViewer(PicsouUIService *ui_svc,
                   QUuid uuid, QWidget *parent=nullptr);

    inline QUuid mod_obj_id() const { return m_uuid; }

private:
    QUuid m_uuid;
};

#endif // PICSOUUIVIEWER_H
