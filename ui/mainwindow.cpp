#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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
            this, &QWidget::close);
    /* view menu */

    /* tools menu */
    connect(ui->action_statistics, &QAction::triggered,
            ui_svc, &PicsouUIService::show_statistics);
    /* help menu */
    connect(ui->action_about_qt, &QAction::triggered,
            qApp, &QApplication::aboutQt);
    connect(ui->action_about_picsou, &QAction::triggered,
            ui_svc, &PicsouUIService::show_about_picsou);
    connect(ui->action_star_me_on_github, &QAction::triggered,
            ui_svc, &PicsouUIService::show_github_repo);

    /* signal handlers */
    connect(ui_svc, &PicsouUIService::db_opened,this, &MainWindow::db_opened);
    connect(ui_svc, &PicsouUIService::db_saved, this, &MainWindow::db_saved);
    connect(ui_svc, &PicsouUIService::db_closed, this, &MainWindow::db_closed);
    connect(ui_svc, &PicsouUIService::op_canceled, this, &MainWindow::op_canceled);
    connect(ui_svc, &PicsouUIService::op_failed, this, &MainWindow::op_failed);
}

void MainWindow::db_opened()
{
    refresh(DB_OPENED);
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
        ui->tree->setEnabled(false);
        /* update mdi */
        ui->mdi->closeAllSubWindows();
        ui->mdi->setEnabled(false);
        break;
    case DB_OPENED:
        /* update menu actions */
        ui->action_save_as->setEnabled(true);
        ui->action_close->setEnabled(true);
        /* update tree widget */
        refresh_tree();
        ui->tree->setEnabled(true);
        /* update mdi */
        ui->mdi->setEnabled(true);
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
}

