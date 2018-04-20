#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>

#include "model/searchquery.h"
#include "ui/picsouuiviewer.h"
#include "app/picsouuiservice.h"

#define TIMEOUT 5000

MainWindow::~MainWindow()
{
    if(_details_widget!=nullptr) {
        delete _details_widget;
    }
    delete _search_ops_stats;
    delete _search_table;
    delete ui;
}

MainWindow::MainWindow(PicsouUIService *ui_svc, QWidget *parent) :
    QMainWindow(parent),
    PicsouUI(ui_svc),
    _state(DB_CLOSED),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _details_widget=nullptr;
    ui->details_tab->setLayout(new QVBoxLayout);

    _search_table=new PicsouTableWidget;
    ui->search_tab->layout()->addWidget(_search_table);

    _search_ops_stats=new OperationStatistics;
    ui->search_tab->layout()->addWidget(_search_ops_stats);

    ui->min_sb->setPrefix(tr("$"));
    ui->min_sb->setSuffix(tr(" "));

    ui->max_sb->setPrefix(tr("$"));
    ui->max_sb->setSuffix(tr(" "));

    ui->to_date->setDate(QDate::currentDate());
    /* initialize window attributes */
    setWindowIcon(QIcon());
    setWindowTitle("Picsou");
    /* initialize connections */
    /* file menu */
    connect(ui->action_new, &QAction::triggered,
            ui_svc, &PicsouUIService::db_new);
    connect(ui->action_open, &QAction::triggered,
            ui_svc, &PicsouUIService::db_open);
    connect(ui->action_close, &QAction::triggered,
            ui_svc, &PicsouUIService::db_close);

    connect(ui->action_save, &QAction::triggered,
            ui_svc, &PicsouUIService::db_save);
    connect(ui->action_save_as, &QAction::triggered,
            ui_svc, &PicsouUIService::db_save_as);

    connect(ui->action_quit, &QAction::triggered,
            this, &MainWindow::close);
    /* edit menu */

    /* help menu */
    connect(ui->action_about_qt, &QAction::triggered,
            qApp, &QApplication::aboutQt);
    connect(ui->action_about_picsou, &QAction::triggered,
            ui_svc, &PicsouUIService::show_about_picsou);
    connect(ui->action_star_me_on_github, &QAction::triggered,
            ui_svc, &PicsouUIService::show_github_repo);

    /* database tree */
    connect(ui->tree, &QTreeWidget::itemSelectionChanged,
            this, &MainWindow::update_viewer);

    /* search-related */
    connect(ui->search_btn, &QPushButton::clicked,
            this, &MainWindow::update_search);
    connect(ui->user_cb, &QComboBox::currentTextChanged,
            this, &MainWindow::refresh_account_cb);
    connect(ui->user_cb, &QComboBox::currentTextChanged,
            this, &MainWindow::refresh_budgets_list);
    connect(ui->account_cb, &QComboBox::currentTextChanged,
            this, &MainWindow::refresh_pms_list);

    /* signal handlers */
    connect(ui_svc, &PicsouUIService::db_opened,
            this, &MainWindow::db_opened);
    connect(ui_svc, &PicsouUIService::db_saved,
            this, &MainWindow::db_saved);
    connect(ui_svc, &PicsouUIService::db_modified,
            this, &MainWindow::db_modified);
    connect(ui_svc, &PicsouUIService::db_closed,
            this, &MainWindow::db_closed);
    connect(ui_svc, &PicsouUIService::svc_op_canceled,
            this, &MainWindow::op_canceled);
    connect(ui_svc, &PicsouUIService::svc_op_failed,
            this, &MainWindow::op_failed);


    refresh(DB_CLOSED);
}

void MainWindow::db_opened()
{
    refresh(DB_OPENED);
}

void MainWindow::db_modified()
{
    refresh(DB_MODIFIED);
}

void MainWindow::db_saved()
{
    refresh(DB_SAVED);
}

void MainWindow::db_closed()
{
    refresh(DB_CLOSED);
}

void MainWindow::op_canceled()
{
    ui->statusbar->showMessage(tr("Operation canceled."), TIMEOUT);
}

void MainWindow::op_failed(QString error)
{
    ui->statusbar->showMessage(tr("Operation failed."), TIMEOUT);
    QMessageBox::critical(this,
                          tr("An error occurred"),
                          error,
                          QMessageBox::Ok,
                          QMessageBox::Ok);
}

void MainWindow::update_viewer()
{
    QList<QTreeWidgetItem*> items;
    items=ui->tree->selectedItems();
    if(items.length()>0) {
        _update_viewer(items.first(), 0);
    }
}

bool MainWindow::close()
{
    ui_svc()->db_close();
    return QWidget::close();
}

void MainWindow::update_search()
{
    OperationCollection ops;
    QStringList selected_budgets, selected_pms;

    foreach (QListWidgetItem *item, ui->budgets->selectedItems()) {
        selected_budgets << item->text();
    }
    foreach (QListWidgetItem *item, ui->pms->selectedItems()) {
        selected_pms << item->text();
    }

    _search_table->clear();
    ops=ui_svc()->search_operations(SearchQuery(ui->user_cb->currentText(),
                                                ui->account_cb->currentText(),
                                                ui->from_date->date(),
                                                ui->to_date->date(),
                                                ui->min_sb->value(),
                                                ui->max_sb->value(),
                                                ui->description_le->text(),
                                                ui->recipient_le->text(),
                                                selected_budgets,
                                                selected_pms));
    if(ops.length()>0) {
        _search_table->refresh(ops);
        _search_ops_stats->refresh(ops);
    }
}

void MainWindow::refresh_user_cb()
{
    ui->user_cb->clear();
    if(!ui_svc()->populate_user_cb(ui->user_cb)) {
        ui->user_cb->clear();
        ui->search_btn->setEnabled(false);
        ui->statusbar->showMessage(tr("Failed to update user combo box."), TIMEOUT);
    }
}

void MainWindow::refresh_account_cb(const QString &username)
{
    ui->account_cb->clear();
    if(!ui_svc()->populate_account_cb(username,
                                      ui->account_cb)) {
        ui->account_cb->clear();
        ui->search_btn->setEnabled(false);
        ui->statusbar->showMessage(tr("Failed to update account combo box."), TIMEOUT);
    }
}

void MainWindow::refresh_budgets_list(const QString &username)
{
    ui->budgets->clear();
    if(!ui_svc()->populate_budgets_list(username,
                                        ui->budgets)) {
        ui->budgets->clear();
        ui->search_btn->setEnabled(false);
        ui->statusbar->showMessage(tr("Failed to update budgets list."), TIMEOUT);
    }
    if(ui->budgets->count()==0) {
        ui->search_btn->setEnabled(false);
    }
    ui->budgets->selectAll();
}

void MainWindow::refresh_pms_list(const QString &account_name)
{
    ui->pms->clear();
    if(!ui_svc()->populate_pms_list(ui->user_cb->currentText(),
                                    account_name,
                                    ui->pms)) {
        ui->pms->clear();
        ui->search_btn->setEnabled(false);
        ui->statusbar->showMessage(tr("Failed to update payment methods list."), TIMEOUT);
    }
    if(ui->pms->count()==0) {
        ui->search_btn->setEnabled(false);
    } else {
        ui->search_btn->setEnabled(true);
    }
    ui->pms->selectAll();
}

void MainWindow::_update_viewer(QTreeWidgetItem *item, int)
{
    PicsouUIViewer *w=ui_svc()->viewer_from_item(item);
    if(w!=nullptr) {
        if(_details_widget!=nullptr) {
            foreach (QAction *action, _details_widget->actions()) {
                ui->toolbar->removeAction(action);
            }
            ui->details_tab->layout()->removeWidget(_details_widget);
            delete _details_widget;
        }
        _details_widget=w;
        ui->details_tab->layout()->addWidget(_details_widget);
        ui->toolbar->addActions(_details_widget->actions());
    }
}

void MainWindow::refresh(MainWindow::State state)
{
    _state=state;
    switch (_state) {
    case DB_CLOSED:
        /* update menu actions */
        ui->action_new->setEnabled(true);
        ui->action_open->setEnabled(true);
        ui->action_save->setEnabled(false);
        ui->action_save_as->setEnabled(false);
        ui->action_close->setEnabled(false);
        /* update tree widget */
        ui->tree->clear();
        ui->tree_dock->setVisible(false);
        /* tab widget */
        ui->tab_widget->setVisible(false);
        break;
    case DB_OPENED:
        /* update menu actions */
        ui->action_save_as->setEnabled(true);
        ui->action_close->setEnabled(true);
        /* update tree widget */
        refresh_tree();
        ui->tree_dock->setVisible(true);
        ui->tab_widget->setVisible(true);
        /* refresh search filters */
        refresh_user_cb();
        /* update viewer */
        _update_viewer(ui->tree->topLevelItem(0), 0);
        break;
    case DB_MODIFIED:
        /* update menu actions */
        ui->action_save->setEnabled(true);
        /* update tree widget */
        refresh_tree();
        /* refresh search filters */
        refresh_user_cb();
        break;
    case DB_SAVED:
        /* update menu actions */
        ui->action_save->setEnabled(false);
        break;
    }
}

void MainWindow::refresh_tree()
{
    ui->tree->clear();
    if(!ui_svc()->populate_db_tree(ui->tree)) {
        ui->tree->clear();
        ui->statusbar->showMessage(tr("Failed to update database tree."), TIMEOUT);
    }
    ui->tree->expandToDepth(1);
}


