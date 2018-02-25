#ifndef SCHEDULEDOPERATIONEDITOR_H
#define SCHEDULEDOPERATIONEDITOR_H

#include <QDialog>

namespace Ui {
class ScheduledOperationEditor;
}

class ScheduledOperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~ScheduledOperationEditor();
    explicit ScheduledOperationEditor(double *amount,
                                      QString *freq,
                                      QDate *date,
                                      QString *payment_method,
                                      QString *budget,
                                      QString *recipient,
                                      QString *description,
                                      QWidget *parent=nullptr);

    void set_budgets(const QStringList &budgets);
    void set_frequencies(const QStringList &frequencies);
    void set_payment_methods(const QStringList &payment_methods);

public slots:
    void accept();

private:
    double *_amount;
    QString *_freq;
    QDate *_date;
    QString *_payment_method;
    QString *_budget;
    QString *_recipient;
    QString *_description;
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
