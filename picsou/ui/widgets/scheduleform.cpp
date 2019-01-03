#include "scheduleform.h"
#include "ui_scheduleform.h"

ScheduleForm::~ScheduleForm()
{
    delete ui;
}

ScheduleForm::ScheduleForm(const Schedule &schedule, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScheduleForm)
{
    ui->setupUi(this);

    ui->freq_unit->addItems(Schedule::TR_FREQUENCY_UNITS);
    ui->freq_unit->setEditable(false);
    ui->freq_unit->setCurrentText(Schedule::freq_unit2trstr(schedule.freq_unit()));

    ui->from->setDate(schedule.from());
    ui->until->setDate(schedule.until());
    ui->endless->setChecked(schedule.endless());

    ui->freq_value->setMinimum(1);
    ui->freq_value->setValue(schedule.freq_value());

    connect(ui->from, &QDateEdit::dateChanged, this, &ScheduleForm::limit_until);
    connect(ui->endless, &QCheckBox::clicked, this, &ScheduleForm::update_endless);
    connect(ui->freq_unit, &QComboBox::currentTextChanged, this, &ScheduleForm::limit_freq);
}

Schedule ScheduleForm::schedule() const
{
    return Schedule(ui->from->date(),
                    ui->until->date(),
                    ui->endless->isChecked(),
                    ui->freq_value->value(),
                    Schedule::trstr2freq_unit(ui->freq_unit->currentText()));
}

void ScheduleForm::update_endless(bool checked)
{
    ui->until->setEnabled(!checked);
}

void ScheduleForm::limit_freq(const QString &freq_unit)
{
    int max;
    switch (Schedule::trstr2freq_unit(freq_unit)) {
        case Schedule::DAY:
            max=364;
            break;
        case Schedule::WEEK:
            max=52;
            break;
        case Schedule::MONTH:
            max=11;
            break;
        case Schedule::YEAR:
            max=INT_MAX;
            break;
    }
    ui->freq_value->setMaximum(max);
}

void ScheduleForm::limit_until(const QDate &from)
{
    ui->until->setMinimumDate(from.addDays(1));
}
