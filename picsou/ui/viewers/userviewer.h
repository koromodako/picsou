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
#ifndef USERVIEWER_H
#define USERVIEWER_H

#include "ui/picsouuiviewer.h"
#include "model/object/picsoudb.h"

namespace Ui {
class UserViewer;
}

class UserViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~UserViewer();
    explicit UserViewer(PicsouUIServicePtr ui_svc,
                        QUuid uuid,
                        QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBShPtr db);

    void add_account();
    void edit_account();
    void remove_account();

    void add_budget();
    void edit_budget();
    void remove_budget();

    void transfer();

private:
    Ui::UserViewer *ui;
};

#endif // USERVIEWER_H
