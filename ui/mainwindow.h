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
        DB_MODIFIED
    };

    virtual ~MainWindow();
    explicit MainWindow(PicsouUIService *ui_svc, QWidget *parent=nullptr);


public slots:
    void db_opened();
    void db_saved();
    void db_modified();
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
    void _update_viewer(QTreeWidgetItem *item, int column);

private:
    void refresh(State state);
    void refresh_tree();

private:
    State _state;
    QWidget *_details_widget;
    PicsouTableWidget *_search_table;
    OperationStatistics *_search_ops_stats;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
