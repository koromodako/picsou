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
#include "scheduledoperationeditor.h"
#include "ui_scheduledoperationeditor.h"

#include "utils/schedule.h"

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}

ScheduledOperationEditor::ScheduledOperationEditor(QWidget *parent,
                                                   const Amount &amount,
                                                   const QString &budget,
                                                   const QString &recipient,
                                                   const QString &description,
                                                   const QString &payment_method,
                                                   const QString &name,
                                                   const Schedule &schedule) :
    QDialog(parent),
    m_amount(amount),
    m_budget(budget),
    m_recipient(recipient),
    m_description(description),
    m_payment_method(payment_method),
    m_name(name),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Scheduled Operation Editor"));

    m_schedule_form=new ScheduleForm(schedule);
    ui->schedule_box->layout()->addWidget(m_schedule_form);

    if(!m_name.isNull()) {
        ui->name->setText(m_name);
    }

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(m_amount.value());

    ui->budget->setEditable(false);
    ui->payment_method->setEditable(false);

    if(!m_recipient.isNull()) {
        ui->recipient->setText(m_recipient);
    }
    if(!m_description.isNull()) {
        ui->description->setPlainText(m_description);
    }

    connect(ui->save, &QPushButton::clicked, this, &ScheduledOperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &ScheduledOperationEditor::reject);
}

void ScheduledOperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItem("");
    ui->budget->addItems(budgets);
    if(!m_budget.isNull()) {
        ui->budget->setCurrentText(m_budget);
    }
}

void ScheduledOperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!m_payment_method.isNull()) {
        ui->payment_method->setCurrentText(m_payment_method);
    }
}

void ScheduledOperationEditor::accept()
{
    m_amount=ui->amount->value();
    m_payment_method=ui->payment_method->currentText();
    m_budget=ui->budget->currentText();
    m_recipient=ui->recipient->text();
    m_description=ui->description->toPlainText();
    m_name=ui->name->text();
    m_schedule=m_schedule_form->schedule();
    QDialog::accept();
}
