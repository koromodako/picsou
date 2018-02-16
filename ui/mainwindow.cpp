#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(PicsouUIService *ui_svc, QWidget *parent) :
    QMainWindow(parent),
    PicsouUI(ui_svc),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

