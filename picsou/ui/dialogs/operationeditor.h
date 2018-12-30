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
#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QDate>
#include <QDialog>
#include "utils/amount.h"

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~OperationEditor();
    explicit OperationEditor(QWidget *parent,
                             int year=-1,
                             int month=-1,
                             bool verified=false,
                             const QDate &date=QDate(),
                             const Amount &amount=Amount(),
                             const QString &budget=QString(),
                             const QString &recipient=QString(),
                             const QString &description=QString(),
                             const QString &payment_method=QString());

    void set_budgets(const QStringList &budgets);
    void set_payment_methods(const QStringList &payment_methods);

    inline bool verified() const { return m_verified; }
    inline QDate date() const { return m_date; }
    inline Amount amount() const { return m_amount; }
    inline QString budget() const { return m_budget; }
    inline QString recipient() const { return m_recipient; }
    inline QString description() const { return m_description; }
    inline QString payment_method() const { return m_payment_method; }

public slots:
    void accept();

private:
    int m_year;
    int m_month;
    bool m_verified;
    QDate m_date;
    Amount m_amount;
    QString m_budget;
    QString m_recipient;
    QString m_description;
    QString m_payment_method;
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
