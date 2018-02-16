#ifndef ACCOUNTVIEWER_H
#define ACCOUNTVIEWER_H

#include <QWidget>

namespace Ui {
class AccountViewer;
}

class AccountViewer : public QWidget
{
    Q_OBJECT

public:
    explicit AccountViewer(QWidget *parent = 0);
    ~AccountViewer();

private:
    Ui::AccountViewer *ui;
};

#endif // ACCOUNTVIEWER_H
