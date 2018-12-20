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
#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QDialog>

namespace Ui {
class UserEditor;
}

class UserEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~UserEditor();
    explicit UserEditor(QWidget *parent,
                        const QString &username = QString(),
                        const QString &old_pswd = QString(),
                        const QString &new_pswd = QString());

    inline QString username() const { return m_username; }
    inline QString old_pswd() const { return m_old_pswd; }
    inline QString new_pswd() const { return m_new_pswd; }

public slots:
    void accept();

private:
    QString m_username;
    QString m_old_pswd;
    QString m_new_pswd;
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
