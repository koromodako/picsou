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
#include "paymentmethodeditor.h"
#include "ui_paymentmethodeditor.h"

PaymentMethodEditor::~PaymentMethodEditor()
{
    delete ui;
}

PaymentMethodEditor::PaymentMethodEditor(QWidget *parent, const QString &name) :
    QDialog(parent),
    m_name(name),
    ui(new Ui::PaymentMethodEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Payment Method Editor"));

    if(!m_name.isNull()) {
        ui->name->setText(m_name);
    }

    connect(ui->save, &QPushButton::clicked, this, &PaymentMethodEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &PaymentMethodEditor::reject);
}

void PaymentMethodEditor::accept()
{
    m_name=ui->name->text();
    QDialog::accept();
}
