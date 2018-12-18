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
#include "picsoudbeditor.h"
#include "ui_picsoudbeditor.h"

PicsouDBEditor::~PicsouDBEditor()
{
    delete ui;
    /* do not delete anything else here */
}

PicsouDBEditor::PicsouDBEditor(QWidget *parent,
                               const QString &name,
                               const QString &description) :
    QDialog(parent),
    m_name(name),
    m_description(description),
    ui(new Ui::PicsouDBEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("PicsouDB Editor"));

    connect(ui->save, &QPushButton::clicked, this, &PicsouDBEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &PicsouDBEditor::reject);
}

void PicsouDBEditor::accept()
{
    m_name=ui->name->text();
    m_description=ui->description->toPlainText();
    QDialog::accept();
}
