#ifndef CHARTSFORM_H
#define CHARTSFORM_H

#include <QWidget>

namespace Ui {
class ChartsForm;
}

class ChartsForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChartsForm(QWidget *parent = nullptr);
    ~ChartsForm();

private:
    Ui::ChartsForm *ui;
};

#endif // CHARTSFORM_H
