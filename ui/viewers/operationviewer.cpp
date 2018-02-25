#include "operationviewer.h"
#include "ui_operationviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoutableitem.h"

OperationViewer::~OperationViewer()
{
    delete ui;
}

OperationViewer::OperationViewer(PicsouUIService *ui_svc,
                                 QUuid user_id,
                                 QUuid account_id,
                                 ViewerScale scale,
                                 int ym,
                                 QWidget *parent) :
    PicsouUIViewer(ui_svc, account_id, parent),
    _user_id(user_id),
    _ym(ym),
    _scale(scale),
    ui(new Ui::OperationViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::model_updated,
            this, &OperationViewer::refresh);

    /* ops */
    connect(ui->add_op, &QPushButton::clicked,
            this, &OperationViewer::add_op);
    connect(ui->edit_op, &QPushButton::clicked,
            this, &OperationViewer::edit_op);
    connect(ui->remove_op, &QPushButton::clicked,
            this, &OperationViewer::remove_op);
}

void OperationViewer::refresh(const PicsouDBPtr db)
{
    int year=-1, month=-1, k;
    QList<OperationPtr> ops;
    QIcon icon;

    ui->table->clear();

    switch (_scale) {
    case VS_YEAR: year=_ym; break;
    case VS_MONTH: month=_ym; break;
    }

    k=0;
    ops=db->ops(mod_obj_id(), year, month);
    foreach (OperationPtr op, ops) {
        switch (op->type()) {
        case Operation::DEBIT: icon=QIcon(); break;
        case Operation::CREDIT: icon=QIcon(); break;
        }

        ui->table->setItem(k, 0, new PicsouTableItem(icon,
                                                     op->date().toString(),
                                                     op->id()));
        ui->table->setItem(k, 1, new QTableWidgetItem(op->description()));
        ui->table->setItem(k, 2, new QTableWidgetItem(op->recipient()));
        ui->table->setItem(k, 3, new QTableWidgetItem(op->payment_method()));
        ui->table->setItem(k, 4, new QTableWidgetItem(op->budget()));
        ui->table->setItem(k, 5, new QTableWidgetItem(op->amount_str()));

        k++;
    }
}

void OperationViewer::add_op()
{
    ui_svc()->op_add(_user_id, mod_obj_id());
}

void OperationViewer::edit_op()
{
    PicsouTableItem *item;
    item=static_cast<PicsouTableItem*>(ui->table->item(ui->table->currentRow(), 0));
    ui_svc()->op_edit(_user_id, mod_obj_id(), item->mod_obj_id());
}

void OperationViewer::remove_op()
{
    PicsouTableItem *item;
    item=static_cast<PicsouTableItem*>(ui->table->item(ui->table->currentRow(), 0));
    ui_svc()->op_remove(mod_obj_id(), item->mod_obj_id());
}
