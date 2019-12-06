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
#include "ui/widgets/scheduleform.h"

class Schedule;

namespace Ui {
class ScheduledOperationEditor;
}

class ScheduledOperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~ScheduledOperationEditor();
    explicit ScheduledOperationEditor(QWidget *parent,
                                      const Amount &amount = Amount(),
                                      const QString &budget = QString(),
                                      const QString &srcdst = QString(),
                                      const QString &description = QString(),
                                      const QString &payment_method = QString(),
                                      const QString &name = QString(),
                                      const Schedule &schedule = Schedule());

    void set_budgets(const QStringList &budgets);
    void set_payment_methods(const QStringList &payment_methods);

    inline Amount amount() const { return m_amount; }
    inline QString budget() const { return m_budget; }
    inline QString srcdst() const { return m_srcdst; }
    inline QString description() const { return m_description; }
    inline QString payment_method() const { return m_payment_method; }
    inline QString name() const { return m_name; }
    inline Schedule schedule() const { return m_schedule; }

public slots:
    void accept();
    void update(bool credit);

private:
    Amount m_amount;
    QString m_budget;
    QString m_srcdst;
    QString m_description;
    QString m_payment_method;
    QString m_name;
    Schedule m_schedule;
    ScheduleForm *m_schedule_form;
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
