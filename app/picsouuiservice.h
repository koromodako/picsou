#ifndef PICSOUUISERVICE_H
#define PICSOUUISERVICE_H

#include "picsouabstractservice.h"

class MainWindow;
class QTreeWidget;

class PicsouUIService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouUIService();
    explicit PicsouUIService(PicsouApplication *papp);

    bool initialize();
    void terminate();

    bool populate_db_tree(QTreeWidget* const tree);

signals:
    void db_opened();
    void db_saved();
    void db_closed();
    void op_failed(QString error);
    void op_canceled();

public slots:
    /*  */
    void show_mainwindow();
    void show_statistics();
    void show_about_picsou();
    void show_github_repo();
    /*  */
    void db_new();
    void db_open();
    void db_close();
    void db_save();
    void db_save_as();

private:
    bool has_opened_db();

private:
    MainWindow *_mw;
};

#endif // PICSOUUISERVICE_H
