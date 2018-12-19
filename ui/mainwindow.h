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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "picsouui.h"
#include "ui/widgets/picsoutablewidget.h"
#include "ui/widgets/operationstatistics.h"

namespace Ui {
class MainWindow;
}

class QTreeWidgetItem;

class MainWindow : public QMainWindow, public PicsouUI
{
    Q_OBJECT

public:
    enum State {
        DB_CLOSED,
        DB_OPENED,
        DB_SAVED,
        DB_MODIFIED,
        DB_UNWRAPPED
    };

    virtual ~MainWindow();
    explicit MainWindow(PicsouUIService *ui_svc, QWidget *parent=nullptr);


public slots:
    void db_opened();
    void db_saved();
    void db_modified();
    void db_unwrapped();
    void db_closed();

    void op_canceled();
    void op_failed(QString error);

    void update_viewer();

    bool close();

    void update_search();

    void refresh_user_cb();
    void refresh_pms_list(const QString &account_name);
    void refresh_account_cb(const QString &username);
    void refresh_budgets_list(const QString &username);

protected:
    void p_update_viewer(QTreeWidgetItem *item, int column);

private:
    void refresh(State state);
    void refresh_tree();

private:
    State m_state;
    QWidget *m_details_widget;
    PicsouTableWidget *m_search_table;
    OperationStatistics *m_search_ops_stats;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
