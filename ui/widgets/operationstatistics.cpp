#include "operationstatistics.h"
#include "ui_operationstatistics.h"

#include <QFormLayout>

OperationStatistics::~OperationStatistics()
{
    _extra_fields.clear();
    delete ui;
}

OperationStatistics::OperationStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationStatistics)
{
    ui->setupUi(this);
}

void OperationStatistics::refresh(const OperationCollection &ops)
{
    int r;
    QHash<QString,Amount> hash;
    QHash<QString,Amount>::const_iterator it;

    ui->balance_val->setText(ops.balance().to_str(true));
    ui->total_debit_val->setText(ops.total_debit().to_str(true));
    ui->total_credit_val->setText(ops.total_credit().to_str(true));

    hash=ops.expense_per_budget();
    ui->expense_per_budget_table->clear();
    ui->expense_per_budget_table->setRowCount(hash.count());
    ui->expense_per_budget_table->setColumnCount(2);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    r=0;
    for(it=hash.begin();it!=hash.end();it++) {
        ui->expense_per_budget_table->setItem(r, 0, new QTableWidgetItem(it.key()));
        ui->expense_per_budget_table->setItem(r, 1, new QTableWidgetItem(it.value().to_str(true)));
        r++;
    }
}

bool OperationStatistics::append_field(const QString &name, const QString &value)
{
    bool success;
    QLabel *lab;
    QFormLayout *flayout;

    if(_extra_fields.contains(name)){
        success=false;
        goto end;
    }

    lab=new QLabel(value, this);
    _extra_fields.insert(name, lab);

    flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->addRow(name, lab);

    success=true;
end:
    return success;
}

bool OperationStatistics::update_field(const QString &name, const QString &value)
{
    bool success;
    QHash<QString, QLabel*>::iterator it;

    if((it=_extra_fields.find(name))==_extra_fields.end()) {
        success=false;
        goto end;
    }

    it.value()->setText(value);

    success=true;
end:
    return success;
}

bool OperationStatistics::remove_field(const QString &name)
{
    bool success;
    QFormLayout *flayout;
    QHash<QString, QLabel*>::iterator it;

    if((it=_extra_fields.find(name))==_extra_fields.end()) {
        success=false;
        goto end;
    }

    flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->removeRow(it.value());

    _extra_fields.erase(it);

    success=true;
end:
    return success;
}
