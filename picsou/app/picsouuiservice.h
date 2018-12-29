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
#ifndef PICSOUUISERVICE_H
#define PICSOUUISERVICE_H

#include "picsouabstractservice.h"
#include "model/object/picsoudb.h"
#include "model/searchquery.h"

class QComboBox;
class MainWindow;
class QListWidget;
class QTreeWidget;
class PicsouUIViewer;
class QTreeWidgetItem;

class PicsouUIService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouUIService();
    explicit PicsouUIService(PicsouApplication *papp);

    bool initialize();
    void terminate();

    bool prompt_for_pswd(const QString &username,
                         QString &pswd,
                         bool confirmed=false);

    bool populate_db_tree(QTreeWidget* const tree);
    bool populate_user_cb(QComboBox * const cb);
    bool populate_account_cb(const QString &username,
                             QComboBox* const cb);
    bool populate_budgets_list(const QString &username,
                               QListWidget* const list);
    bool populate_pms_list(const QString &username,
                           const QString &account_name,
                           QListWidget* const list);

    OperationCollection search_operations(const SearchQuery &query);

    PicsouUIViewer *viewer_from_item(QTreeWidgetItem *item);

signals:
    void svc_op_failed(QString error);
    void svc_op_canceled();

    void notify_model_updated(const PicsouDBShPtr db);
    void notify_model_unwrapped(const PicsouDBShPtr db);

    void unlocked();

    void db_opened();
    void db_saved();
    void db_modified();
    void db_unwrapped();
    void db_closed();

    void user_added();
    void user_edited();
    void user_removed();
    /* Budget ops */
    void budget_added();
    void budget_edited();
    void budget_removed();
    /* Account ops */
    void account_added();
    void account_edited();
    void account_removed();
    /* Payment Methods ops */
    void pm_added();
    void pm_edited();
    void pm_removed();
    /* Scheduled Operation ops */
    void sop_added();
    void sop_edited();
    void sop_removed();
    /* Operation ops */
    void op_added();
    void op_edited();
    void op_removed();
    void ops_imported();
    void ops_exported();

public slots:
    /*  */
    void show_mainwindow();
    void show_statistics();
    void show_about_picsou();
    void show_github_repo();
    void show_license();
    /*  */
    void unlock(QUuid id);
    /* DB ops */
    void db_new();
    void db_open();
    void db_close();
    void db_save();
    void db_save_as();
    /* User ops */
    void user_add();
    void user_edit(QUuid id);
    void user_remove(QUuid id);
    /* Budget ops */
    void budget_add(QUuid user_id);
    void budget_edit(QUuid user_id, QUuid budget_id);
    void budget_remove(QUuid user_id, QUuid budget_id);
    /* Account ops */
    void account_add(QUuid user_id);
    void account_edit(QUuid user_id, QUuid account_id);
    void account_remove(QUuid user_id, QUuid account_id);
    /* Payment Methods ops */
    void pm_add(QUuid account_id);
    void pm_edit(QUuid account_id, QUuid pm_id);
    void pm_remove(QUuid account_id, QUuid pm_id);
    /* Scheduled Operation ops */
    void sop_add(QUuid user_id, QUuid account_id);
    void sop_edit(QUuid user_id, QUuid account_id, QUuid pm_id);
    void sop_remove(QUuid account_id, QUuid pm_id);
    /* Operation ops */
    void op_add(QUuid user_id, QUuid account_id, int year, int month);
    void op_edit(QUuid user_id, QUuid account_id, QUuid op_id, int year, int month);
    void op_remove(QUuid account_id, QUuid op_id);
    void ops_import(QUuid account_id);
    void ops_export(QUuid account_id);

    void notified_model_updated(const PicsouDBShPtr db);
    void notified_model_unwrapped(const PicsouDBShPtr db);

private:
    bool close_any_opened_db();

private:
    MainWindow *m_mw;
};

typedef QPointer<PicsouUIService> PicsouUIServicePtr;

#endif // PICSOUUISERVICE_H
