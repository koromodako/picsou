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
#include "accounteditor.h"
#include "ui_accounteditor.h"

AccountEditor::~AccountEditor()
{
    delete ui;
}

AccountEditor::AccountEditor(QWidget *parent,
                             const QString &name,
                             const QString &notes) :
    QDialog(parent),
    m_name(name),
    m_notes(notes),
    ui(new Ui::AccountEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Account Editor"));

    if(!m_name.isNull()) {
        ui->name->setText(m_name);
    }
    if(!m_notes.isNull()) {
        ui->description->setPlainText(m_notes);
    }

    connect(ui->save, &QPushButton::clicked, this, &AccountEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &AccountEditor::reject);
}

void AccountEditor::accept()
{
    m_name=ui->name->text();
    m_notes=ui->description->toPlainText();
    QDialog::accept();
}



