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
#include "usereditor.h"
#include "ui_usereditor.h"

#include "utils/macro.h"

UserEditor::~UserEditor()
{
    delete ui;
}

UserEditor::UserEditor(QWidget *parent,
                       const QString &username,
                       const QString &old_pswd,
                       const QString &new_pswd) :
    QDialog(parent),
    m_username(username),
    m_old_pswd(old_pswd),
    m_new_pswd(new_pswd),
    ui(new Ui::UserEditor)
{
    bool user_edit=false;
    ui->setupUi(this);

    setWindowTitle(tr("User Editor"));

    ui->error->setVisible(false);

    ui->old_pswd->setEchoMode(QLineEdit::Password);
    ui->new_pswd->setEchoMode(QLineEdit::Password);
    ui->repeat_new_pswd->setEchoMode(QLineEdit::Password);

    ui->old_pswd->setInputMethodHints(Qt::ImhSensitiveData);
    ui->new_pswd->setInputMethodHints(Qt::ImhSensitiveData);
    ui->repeat_new_pswd->setInputMethodHints(Qt::ImhSensitiveData);

    if((user_edit=!m_username.isNull())) {
        ui->username->setText(m_username);
    }

    ui->old_pswd->setVisible(user_edit);
    ui->old_pswd_label->setVisible(user_edit);

    connect(ui->save, &QPushButton::clicked, this, &UserEditor::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &UserEditor::reject);
}

void UserEditor::accept()
{
    ui->error->setVisible(false);

    if(ui->new_pswd->text()!=ui->repeat_new_pswd->text()) {
        ui->error->setVisible(true);
        return;
    }

    m_old_pswd=ui->old_pswd->text();
    m_new_pswd=ui->new_pswd->text();
    m_username=ui->username->text();
    QDialog::accept();
}
