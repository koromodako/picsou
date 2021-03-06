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
#ifndef PICSOUDBEDITOR_H
#define PICSOUDBEDITOR_H

#include <QDialog>

namespace Ui {
class PicsouDBEditor;
}

class PicsouDBEditor : public QDialog
{
    Q_OBJECT

public:
    virtual ~PicsouDBEditor();
    explicit PicsouDBEditor(QWidget *parent,
                            const QString &name = QString(),
                            const QString &description = QString());

    inline QString name() const { return m_name; }
    inline QString description() const { return m_description; }

public slots:
    void accept();

private:
    QString m_name;
    QString m_description;
    Ui::PicsouDBEditor *ui;
};

#endif // PICSOUDBEDITOR_H
