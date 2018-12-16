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

AccountEditor::AccountEditor(QString *name,
                             QString *description,
                             QWidget *parent) :
    QDialog(parent),
    _name(name),
    _description(description),
    ui(new Ui::AccountEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Account Editor"));

    if(!_name->isNull()) {
        ui->name->setText(*_name);
    }
    if(!_description->isNull()) {
        ui->description->setPlainText(*_description);
    }

    connect(ui->save, &QPushButton::clicked,
            this, &AccountEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &AccountEditor::reject);
}

void AccountEditor::accept()
{
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}



