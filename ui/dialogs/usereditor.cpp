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
    /* do not delete private members here */
}

UserEditor::UserEditor(QString *username,
                       QString *old_pwd,
                       QString *new_pwd,
                       QWidget *parent) :
    QDialog(parent),
    _username(username),
    _old_pwd(old_pwd),
    _new_pwd(new_pwd),
    ui(new Ui::UserEditor)
{
    bool user_edit=false;
    ui->setupUi(this);

    setWindowTitle(tr("User Editor"));

    ui->error->setVisible(false);

    ui->old_pwd->setEchoMode(QLineEdit::Password);
    ui->new_pwd->setEchoMode(QLineEdit::Password);
    ui->repeat_new_pwd->setEchoMode(QLineEdit::Password);

    ui->old_pwd->setInputMethodHints(Qt::ImhSensitiveData);
    ui->new_pwd->setInputMethodHints(Qt::ImhSensitiveData);
    ui->repeat_new_pwd->setInputMethodHints(Qt::ImhSensitiveData);

    if((user_edit=!_username->isNull())) {
        ui->username->setText(*_username);
    }

    ui->old_pwd->setVisible(user_edit);
    ui->old_pwd_label->setVisible(user_edit);

    connect(ui->save, &QPushButton::clicked,
            this, &UserEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &UserEditor::reject);
}

void UserEditor::accept()
{
    ui->error->setVisible(false);

    if(ui->new_pwd->text()!=ui->repeat_new_pwd->text()) {
        ui->error->setVisible(true);
        return;
    }

    (*_old_pwd)=ui->old_pwd->text();
    (*_new_pwd)=ui->new_pwd->text();
    (*_username)=ui->username->text();
    QDialog::accept();
}
