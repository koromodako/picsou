#include "importdialog.h"
#include "ui_importdialog.h"

ImportDialog::~ImportDialog()
{
    delete _table;
    delete ui;
}

ImportDialog::ImportDialog(QList<OperationPtr> ops,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

    _table = new PicsouTableWidget;
    ui->main_layout->insertWidget(0, _table);

    _table->refresh(ops);

    connect(ui->save, &QPushButton::clicked,
            this, &ImportDialog::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &ImportDialog::reject);
}

