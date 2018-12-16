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
                                                   QString *payment_method,
                                                   QString *budget,
                                                   QString *recipient,
                                                   QString *description,
                                                   QString *name,
                                                   Schedule *schedule,
                                                   QWidget *parent) :
    QDialog(parent),
    _amount(amount),
    _payment_method(payment_method),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    _name(name),
    _schedule(schedule),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Scheduled Operation Editor"));

    if(!_name->isNull()) {
        ui->name->setText(*_name);
    }

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(_amount->value());

    ui->budget->setEditable(false);
    ui->payment_method->setEditable(false);

    if(!_recipient->isNull()) {
        ui->recipient->setText(*_recipient);
    }
    if(!_description->isNull()) {
        ui->description->setPlainText(*_description);
    }

    ui->from->setDate(_schedule->from());
    ui->until->setDate(_schedule->until());
    ui->endless->setChecked(_schedule->endless());

    ui->freq_value->setMinimum(1);
    ui->freq_value->setValue(_schedule->freq_value());
    ui->freq_unit->setEditable(false);

    connect(ui->from, &QDateEdit::dateChanged,
            this, &ScheduledOperationEditor::limit_until);
    connect(ui->endless, &QCheckBox::clicked,
            this, &ScheduledOperationEditor::endless);
    connect(ui->freq_unit, &QComboBox::currentTextChanged,
            this, &ScheduledOperationEditor::limit_freq);
    connect(ui->save, &QPushButton::clicked,
            this, &ScheduledOperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &ScheduledOperationEditor::reject);
}

void ScheduledOperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
    if(!_budget->isNull()) {
        ui->budget->setCurrentText(*_budget);
    }
}

void ScheduledOperationEditor::set_frequency_units(const QStringList &frequency_units)
{
    ui->freq_unit->clear();
    ui->freq_unit->addItems(frequency_units);
    ui->freq_unit->setCurrentText(Schedule::freq_unit2str(_schedule->freq_unit()));
}

void ScheduledOperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!_payment_method->isNull()) {
        ui->payment_method->setCurrentText(*_payment_method);
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
    (*_amount)=ui->amount->value();
    (*_payment_method)=ui->payment_method->currentText();
    (*_budget)=ui->budget->currentText();
    (*_recipient)=ui->recipient->text();
    (*_description)=ui->description->toPlainText();
    (*_name)=ui->name->text();
    _schedule->update(ui->from->date(),
                      ui->until->date(),
                      ui->endless->isChecked(),
                      ui->freq_value->value(),
                      Schedule::str2freq_unit(ui->freq_unit->currentText()));
    QDialog::accept();
}
