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

#include <QDialog>
#include "model/object/amount.h"

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~OperationEditor();
    explicit OperationEditor(QDate *date,
                             Amount *amount,
                             QString *payment_method,
                             QString *budget,
                             QString *recipient,
                             QString *description,
                             int year=-1,
                             int month=-1,
                             QWidget *parent=nullptr);

    void set_budgets(const QStringList &budgets);
    void set_payment_methods(const QStringList &payment_methods);

public slots:
    void accept();

private:
    QDate *_date;
    Amount *_amount;
    QString *_payment_method;
    QString *_budget;
    QString *_recipient;
    QString *_description;
    int _year;
    int _month;
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
