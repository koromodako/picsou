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
#ifndef PICSOUDBVIEWER_H
#define PICSOUDBVIEWER_H

#include <QUuid>
#include <QWidget>
#include "ui/picsouuiviewer.h"
#include "model/object/picsoudb.h"

namespace Ui {
class PicsouDBViewer;
}

class PicsouDBViewer : public PicsouUIViewer
{
    Q_OBJECT
public:
    virtual ~PicsouDBViewer();
    explicit PicsouDBViewer(PicsouUIService *ui_svc,
                            QUuid uuid,
                            QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    void add_user();
    void edit_user();
    void remove_user();

private:
    Ui::PicsouDBViewer *ui;
};

#endif // PICSOUDBVIEWER_H
