#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include "utils/amount.h"
#include "utils/schedule.h"

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    virtual ~TransferDialog();
    TransferDialog(QWidget *parent=nullptr);

    void set_accounts(const QStringList &accounts);

    QString sender() const { return m_sender; }
    QString recipient() const { return m_recipient; }
    Amount amount() const { return m_amount; }
    QString description() const { return m_description; }
    Schedule schedule() const { return m_schedule; }

    bool scheduled() const;

public slots:
    void accept();

private:
    QString m_sender;
    QString m_recipient;
    Amount m_amount;
    QString m_description;
    Schedule m_schedule;
    Ui::TransferDialog *ui;
};

#endif // TRANSFERDIALOG_H
