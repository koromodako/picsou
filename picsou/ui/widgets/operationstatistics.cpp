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

void OperationStatistics::refresh(const QString &balance,
                                  const QString &total_debit,
                                  const QString &total_credit,
                                  const QList<QStringList> &budgets)
{
    ui->balance_val->setText(balance);
    ui->total_debit_val->setText(total_debit);
    ui->total_credit_val->setText(total_credit);
    /* clear previous table */
    ui->expense_per_budget_table->clear();
    /* set columns */
    ui->expense_per_budget_table->setColumnCount(budgets.first().size());
    int c=0;
    do {
        ui->expense_per_budget_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
        c++;
    } while(c<budgets.first().size());
    /* fill rows */
    ui->expense_per_budget_table->setRowCount(budgets.size());
    int r=0;
    for(auto strlst : budgets) {
        c=0;
        do {
            ui->expense_per_budget_table->setItem(r, c, new QTableWidgetItem(strlst.at(c)));
            c++;
        } while(c<strlst.size());
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
