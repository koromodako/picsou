#ifndef SCHEDULEFORM_H
#define SCHEDULEFORM_H

#include <QWidget>
#include "utils/schedule.h"

namespace Ui {
class ScheduleForm;
}

class ScheduleForm : public QWidget
{
    Q_OBJECT

public:
    virtual ~ScheduleForm();
    explicit ScheduleForm(const Schedule &schedule=Schedule(),
                          QWidget *parent=nullptr);

    Schedule schedule() const;

private slots:
    void update_endless(bool checked);
    void limit_freq(const QString &freq_unit);
    void limit_until(const QDate &from);

private:
    Ui::ScheduleForm *ui;
};

#endif // SCHEDULEFORM_H
