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
#ifndef SCHEDULEDOPERATIONEDITOR_H
#define SCHEDULEDOPERATIONEDITOR_H

#include <QDialog>
#include "utils/amount.h"

class Schedule;

namespace Ui {
class ScheduledOperationEditor;
}

class ScheduledOperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~ScheduledOperationEditor();
    explicit ScheduledOperationEditor(Amount *amount,
                                      QString *budget,
                                      QString *recipient,
                                      QString *description,
                                      QString *payment_method,
                                      QString *name,
                                      Schedule *schedule,
                                      QWidget *parent=nullptr);

    void set_budgets(const QStringList &budgets);
    void set_frequency_units(const QStringList &frequency_units);
    void set_payment_methods(const QStringList &payment_methods);

public slots:
    void endless(bool checked);
    void limit_freq(const QString &freq_unit);
    void limit_until(const QDate &from);
    void accept();

private:
    Amount *m_amount;
    QString *m_payment_method;
    QString *m_budget;
    QString *m_recipient;
    QString *m_description;
    QString *m_name;
    Schedule *m_schedule;
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
