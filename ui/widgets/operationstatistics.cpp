/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "operationstatistics.h"
#include "ui_operationstatistics.h"

#include <algorithm>

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

bool budget_amount_cmp(const QPair<QString, Amount> &a, const QPair<QString, Amount> &b)
{
    return a.second<b.second;
}

void OperationStatistics::refresh(const OperationCollection &ops)
{
    ui->balance_val->setText(ops.balance().to_str(true));
    ui->total_debit_val->setText(ops.total_debit().to_str(true));
    ui->total_credit_val->setText(ops.total_credit().to_str(true));

    QHash<QString,Amount> hash=ops.expense_per_budget();
    ui->expense_per_budget_table->clear();
    ui->expense_per_budget_table->setRowCount(hash.count());
    ui->expense_per_budget_table->setColumnCount(2);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    QList<QPair<QString, Amount>> sorted_budgets;
    for(QHash<QString,Amount>::const_iterator it=hash.begin();it!=hash.end();it++) {
        sorted_budgets.append(qMakePair(it.key(), it.value()));
    }

    std::sort(sorted_budgets.begin(), sorted_budgets.end(), budget_amount_cmp);

    int r=0;
    for(auto pair : sorted_budgets) {
        ui->expense_per_budget_table->setItem(r, 0, new QTableWidgetItem(pair.first));
        ui->expense_per_budget_table->setItem(r, 1, new QTableWidgetItem(pair.second.to_str(true)));
        r++;
    }
}

bool OperationStatistics::append_field(const QString &name, const QString &value)
{
    if(_extra_fields.contains(name)){
        return false;
    }
    QLabel *lab=new QLabel(value, this);
    _extra_fields.insert(name, lab);
    QFormLayout *flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->addRow(name, lab);
    return true;
}

bool OperationStatistics::update_field(const QString &name, const QString &value)
{
    QHash<QString, QLabel*>::iterator it=_extra_fields.find(name);
    if(it==_extra_fields.end()) {
        return false;
    }
    it.value()->setText(value);
    return true;
}

bool OperationStatistics::remove_field(const QString &name)
{
    QHash<QString, QLabel*>::iterator it=_extra_fields.find(name);
    if(it==_extra_fields.end()) {
        return false;
    }
    QFormLayout *flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->removeRow(it.value());
    _extra_fields.erase(it);
    return true;
}
