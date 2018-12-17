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
#include "budgeteditor.h"
#include "ui_budgeteditor.h"

BudgetEditor::~BudgetEditor()
{
    delete ui;
}


BudgetEditor::BudgetEditor(double *amount,
                           QString *name,
                           QString *description,
                           QWidget *parent) :
    QDialog(parent),
    m_amount(amount),
    m_name(name),
    m_description(description),
    ui(new Ui::BudgetEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Budget Editor"));

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(*m_amount);

    if(!m_name->isNull()) {
        ui->name->setText(*m_name);
    }
    if(!m_description->isNull()) {
        ui->description->setPlainText(*m_description);
    }

    connect(ui->save, &QPushButton::clicked, this, &BudgetEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &BudgetEditor::reject);
}

void BudgetEditor::accept()
{
    (*m_amount)=ui->amount->value();
    (*m_name)=ui->name->text();
    (*m_description)=ui->description->toPlainText();
    QDialog::accept();
}
