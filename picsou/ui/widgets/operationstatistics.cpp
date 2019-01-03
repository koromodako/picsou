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
    m_extra_fields.clear();
    delete ui;
}

OperationStatistics::OperationStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationStatistics)
{
    ui->setupUi(this);
}

void OperationStatistics::clear()
{
    ui->balance_val->setText("-");
    ui->total_debit_val->setText("-");
    ui->total_credit_val->setText("-");
}

static bool budget_amount_cmp(const QPair<QString, Amount> &a, const QPair<QString, Amount> &b)
{
    return a.second<b.second;
}

void OperationStatistics::refresh(const OperationCollection &ops, const BudgetShPtrList &user_budgets)
{
    QHash<QString, Amount> epb=ops.expense_per_budget();
    QList<QPair<QString, Amount>> sorted_epb;
    QHash<QString, Amount>::iterator epb_it=epb.begin();
    while(epb_it!=epb.end()) {
        sorted_epb.append(qMakePair(epb_it.key(), epb_it.value()));
        epb_it++;
    }

    std::sort(sorted_epb.begin(), sorted_epb.end(), budget_amount_cmp);
    QHash<QString, Amount> budget_hash;
    for(const auto &budget : user_budgets) {
        budget_hash.insert(budget->name(), budget->amount());
    }

    ui->balance_val->setText(ops.balance().to_str(true));
    ui->total_debit_val->setText(ops.total_debit().to_str(true));
    ui->total_credit_val->setText(ops.total_credit().to_str(true));
    /* clear previous budget table */
    ui->expense_per_budget_table->clear();
    /* fill table with new values */
    static QStringList labels=QStringList()<<tr("Name")
                                           <<tr("Current Debit")
                                           <<tr("Maximum Debit")
                                           <<tr("Available Debit");

    ui->expense_per_budget_table->setColumnCount(4);
    ui->expense_per_budget_table->setHorizontalHeaderLabels(labels);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->expense_per_budget_table->setRowCount(sorted_epb.length());
    int r=0;
    for(const auto &budget : sorted_epb) {
        Amount allowed=budget_hash.value(budget.first)*ops.months();
        Amount consumed=budget.second;
        Amount remaining=allowed+consumed; /* consumed should be < 0 */
        double remaining_pc=100*remaining.value()/allowed.value();
        if(budget.first.isEmpty()) {
            ui->expense_per_budget_table->setItem(r, 0, new QTableWidgetItem(tr("Unassigned budget")));
        } else {
            ui->expense_per_budget_table->setItem(r, 0, new QTableWidgetItem(budget.first));
        }
        ui->expense_per_budget_table->setItem(r, 1, new QTableWidgetItem(consumed.to_str(true)));
        if(allowed!=0) {
            ui->expense_per_budget_table->setItem(r, 2, new QTableWidgetItem(allowed.to_str(true)));
            ui->expense_per_budget_table->setItem(r, 3, new QTableWidgetItem(QString("%0 (%1%)").arg(remaining.to_str(true),
                                                                                                     QString::number(remaining_pc, 'f', 2))));
        } else {
            ui->expense_per_budget_table->setItem(r, 2, new QTableWidgetItem("N/A"));
            ui->expense_per_budget_table->setItem(r, 3, new QTableWidgetItem("N/A"));
        }
        r++;
    }
}

bool OperationStatistics::append_field(const QString &name, const QString &value)
{
    if(m_extra_fields.contains(name)){
        return false;
    }
    QLabel *lab=new QLabel(value, this);
    m_extra_fields.insert(name, lab);
    QFormLayout *flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->addRow(name, lab);
    return true;
}

bool OperationStatistics::update_field(const QString &name, const QString &value)
{
    QHash<QString, QLabel*>::iterator it=m_extra_fields.find(name);
    if(it==m_extra_fields.end()) {
        return false;
    }
    it.value()->setText(value);
    return true;
}

bool OperationStatistics::remove_field(const QString &name)
{
    QHash<QString, QLabel*>::iterator it=m_extra_fields.find(name);
    if(it==m_extra_fields.end()) {
        return false;
    }
    QFormLayout *flayout=static_cast<QFormLayout*>(ui->balance_box->layout());
    flayout->removeRow(it.value());
    m_extra_fields.erase(it);
    return true;
}
