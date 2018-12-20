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
#ifndef SEARCHFILTERFORM_H
#define SEARCHFILTERFORM_H

#include "ui/picsouui.h"
#include "model/searchquery.h"
#include <QWidget>

namespace Ui {
class SearchFilterForm;
}

class SearchFilterForm : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~SearchFilterForm();
    explicit SearchFilterForm(PicsouUIServicePtr ui_svc,
                              QWidget *parent = nullptr);

    SearchQuery query() const;

signals:
    void search_request();
    void search_update_failed(const QString &message);

public slots:
    void refresh_user_cb();
    void refresh_pms_list(const QString &account_name);
    void refresh_account_cb(const QString &username);
    void refresh_budgets_list(const QString &username);

private:
    Ui::SearchFilterForm *ui;
};

#endif // SEARCHFILTERFORM_H
