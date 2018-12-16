#ifndef SCHEDULEDOPERATIONEDITOR_H
#define SCHEDULEDOPERATIONEDITOR_H

#include <QDialog>
#include "model/object/amount.h"

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
                                      QString *payment_method,
                                      QString *budget,
                                      QString *recipient,
                                      QString *description,
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
    Amount *_amount;
    QString *_payment_method;
    QString *_budget;
    QString *_recipient;
    QString *_description;
    QString *_name;
    Schedule *_schedule;
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
