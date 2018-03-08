#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include "ui/picsouuiviewer.h"
#include "app/picsouuiservice.h"

#define TIMEOUT 5000

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(PicsouUIService *ui_svc, QWidget *parent) :
    QMainWindow(parent),
    PicsouUI(ui_svc),
    _state(DB_CLOSED),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* initialize connections */
    setWindowIcon(QIcon());
    setWindowTitle("Picsou");
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
    connect(ui->tree, &QTreeWidget::itemClicked,
            this, &MainWindow::update_viewer);

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

void MainWindow::update_viewer(QTreeWidgetItem *item, int)
{
    QWidget *before;
    PicsouUIViewer *w=ui_svc()->viewer_from_item(item);
    if(w!=nullptr) {
        if((before=centralWidget())!=nullptr) {
            foreach (QAction *action, before->actions()) {
                ui->toolbar->removeAction(action);
            }
        }
        setCentralWidget(w);
        ui->toolbar->addActions(w->actions());
    }
}

bool MainWindow::close()
{
    ui_svc()->db_close();
    return QWidget::close();
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
        /* central widget */
        setCentralWidget(nullptr);
        break;
    case DB_OPENED:
        /* update menu actions */
        ui->action_save_as->setEnabled(true);
        ui->action_close->setEnabled(true);
        /* update tree widget */
        refresh_tree();
        ui->tree_dock->setVisible(true);
        update_viewer(ui->tree->topLevelItem(0), 0);
        break;
    case DB_MODIFIED:
        /* update menu actions */
        ui->action_save->setEnabled(true);
        /* update tree widget */
        refresh_tree();
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
    ui->tree->expandToDepth(3);
}

