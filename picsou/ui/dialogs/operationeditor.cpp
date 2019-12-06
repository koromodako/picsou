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
#include "operationeditor.h"
#include "ui_operationeditor.h"

OperationEditor::~OperationEditor()
{
    delete ui;
}

OperationEditor::OperationEditor(QWidget *parent,
                                 int year,
                                 int month,
                                 const QStringList &srcdst_candidates,
                                 bool verified,
                                 const QDate &date,
                                 const Amount &amount,
                                 const QString &budget,
                                 const QString &srcdst,
                                 const QString &description,
                                 const QString &payment_method) :
    QDialog(parent),
    m_year(year),
    m_month(month),
    m_verified(verified),
    m_date(date),
    m_amount(amount),
    m_budget(budget),
    m_srcdst(srcdst),
    m_description(description),
    m_payment_method(payment_method),
    m_srcdst_comp(srcdst_candidates),
    ui(new Ui::OperationEditor)
{

    ui->setupUi(this);

    setWindowTitle(tr("Operation Editor"));

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setMinimum(0);
    ui->amount->setMaximum(9999999999);
    ui->amount->setValue(m_amount.absvalue());

    ui->debit_button->setText(tr("Debit"));
    ui->debit_button->setChecked(!m_amount.credit());
    ui->credit_button->setText(tr("Credit"));
    ui->credit_button->setChecked(m_amount.credit());
    update(m_amount.credit());

    ui->verified->setChecked(m_verified);

    if(!m_date.isNull()){
        ui->date->setDate(m_date);
    } else {
        ui->date->setDate(QDate::currentDate());
    }

    QDate min, max;
    if(m_year>0) {
        min.setDate(m_year, 1, 1);
        max.setDate(m_year, 12, 31);

        if(m_month>0) {
            min.setDate(m_year, m_month, 1);
            max.setDate(m_year, m_month, min.daysInMonth());
        }

        ui->date->setDateRange(min, max);
    }

    ui->budget->setEditable(false);
    ui->payment_method->setEditable(false);

    if(!m_srcdst.isNull()){
        ui->srcdst->setText(m_srcdst);
    }

    m_srcdst_comp.setFilterMode(Qt::MatchStartsWith);
    m_srcdst_comp.setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    m_srcdst_comp.setCompletionMode(QCompleter::PopupCompletion);
    ui->srcdst->setCompleter(&m_srcdst_comp);

    if(!m_description.isNull()){
        ui->description->setPlainText(m_description);
    }

    connect(ui->save, &QPushButton::clicked, this, &OperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &OperationEditor::reject);
    connect(ui->credit_button, &QRadioButton::toggled, this, &OperationEditor::update);
}

void OperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItem("");
    ui->budget->addItems(budgets);
    if(!m_budget.isNull()){
        ui->budget->setCurrentText(m_budget);
    }
}

void OperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!m_payment_method.isNull()){
        ui->payment_method->setCurrentText(m_payment_method);
    }
}

void OperationEditor::accept()
{
    m_verified=ui->verified->isChecked();
    m_date=ui->date->date();
    m_amount=(ui->debit_button->isChecked()?-1:1) * ui->amount->value();
    m_payment_method=ui->payment_method->currentText();
    m_budget=ui->budget->currentText();
    m_srcdst=ui->srcdst->text();
    m_description=ui->description->toPlainText();
    QDialog::accept();
}

void OperationEditor::update(bool credit_checked)
{
    ui->srcdst_label->setText((credit_checked?tr("Source:"):tr("Destination:")));
}
