#ifndef PICSOUUISERVICE_H
#define PICSOUUISERVICE_H

#include "picsouabstractservice.h"
#include "model/picsoudb.h"

class MainWindow;
class QTreeWidget;
class QTreeWidgetItem;

class PicsouUIService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouUIService();
    explicit PicsouUIService(PicsouApplication *papp);

    bool initialize();
    void terminate();

    bool populate_db_tree(QTreeWidget* const tree);
    QWidget *viewer_from_item(QTreeWidgetItem *item);

signals:
    void db_opened();
    void db_saved();
    void db_closed();
    void op_failed(QString error);
    void op_canceled();

    void model_updated(const PicsouDBPtr db);

public slots:
    /*  */
    void show_mainwindow();
    void show_statistics();
    void show_about_picsou();
    void show_github_repo();
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
    /* Operation ops */
    void op_add(QUuid account_id);
    void op_edit(QUuid account_id, QUuid op_id);
    void op_remove(QUuid account_id, QUuid op_id);

private:
    bool has_opened_db();

private:
    MainWindow *_mw;
};

#endif // PICSOUUISERVICE_H
