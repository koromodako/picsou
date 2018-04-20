#include "operationstatistics.h"
#include "ui_operationstatistics.h"

OperationStatistics::~OperationStatistics()
{
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

