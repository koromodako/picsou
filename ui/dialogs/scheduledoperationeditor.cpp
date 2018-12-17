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

#include "model/object/schedule.h"

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}

ScheduledOperationEditor::ScheduledOperationEditor(Amount *amount,
                                                   QString *budget,
                                                   QString *recipient,
                                                   QString *description,
                                                   QString *payment_method,
                                                   QString *name,
                                                   Schedule *schedule,
                                                   QWidget *parent) :
    QDialog(parent),
    m_amount(amount),
    m_payment_method(payment_method),
    m_budget(budget),
    m_recipient(recipient),
    m_description(description),
    m_name(name),
    m_schedule(schedule),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Scheduled Operation Editor"));

    if(!m_name->isNull()) {
        ui->name->setText(*m_name);
    }

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(m_amount->value());

    ui->budget->setEditable(false);
    ui->payment_method->setEditable(false);

    if(!m_recipient->isNull()) {
        ui->recipient->setText(*m_recipient);
    }
    if(!m_description->isNull()) {
        ui->description->setPlainText(*m_description);
    }

    ui->from->setDate(m_schedule->from());
    ui->until->setDate(m_schedule->until());
    ui->endless->setChecked(m_schedule->endless());

    ui->freq_value->setMinimum(1);
    ui->freq_value->setValue(m_schedule->freq_value());
    ui->freq_unit->setEditable(false);

    connect(ui->from, &QDateEdit::dateChanged, this, &ScheduledOperationEditor::limit_until);
    connect(ui->endless, &QCheckBox::clicked, this, &ScheduledOperationEditor::endless);
    connect(ui->freq_unit, &QComboBox::currentTextChanged, this, &ScheduledOperationEditor::limit_freq);

    connect(ui->save, &QPushButton::clicked, this, &ScheduledOperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &ScheduledOperationEditor::reject);
}

void ScheduledOperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
    if(!m_budget->isNull()) {
        ui->budget->setCurrentText(*m_budget);
    }
}

void ScheduledOperationEditor::set_frequency_units(const QStringList &frequency_units)
{
    ui->freq_unit->clear();
    ui->freq_unit->addItems(frequency_units);
    ui->freq_unit->setCurrentText(Schedule::freq_unit2str(m_schedule->freq_unit()));
}

void ScheduledOperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!m_payment_method->isNull()) {
        ui->payment_method->setCurrentText(*m_payment_method);
    }
}

void ScheduledOperationEditor::endless(bool checked)
{
    ui->until->setEnabled(!checked);
}

void ScheduledOperationEditor::limit_freq(const QString &freq_unit)
{
    int max;
    switch (Schedule::str2freq_unit(freq_unit)) {
        case Schedule::DAY:
            max=364;
            break;
        case Schedule::WEEK:
            max=52;
            break;
        case Schedule::MONTH:
            max=11;
            break;
        case Schedule::YEAR:
            max=INT_MAX;
            break;
    }
    ui->freq_value->setMaximum(max);
}

void ScheduledOperationEditor::limit_until(const QDate &from)
{
    ui->until->setMinimumDate(from.addDays(1));
}

void ScheduledOperationEditor::accept()
{
    (*m_amount)=ui->amount->value();
    (*m_payment_method)=ui->payment_method->currentText();
    (*m_budget)=ui->budget->currentText();
    (*m_recipient)=ui->recipient->text();
    (*m_description)=ui->description->toPlainText();
    (*m_name)=ui->name->text();
    m_schedule->update(ui->from->date(),
                       ui->until->date(),
                       ui->endless->isChecked(),
                       ui->freq_value->value(),
                       Schedule::str2freq_unit(ui->freq_unit->currentText()));
    QDialog::accept();
}
