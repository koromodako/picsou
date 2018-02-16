#ifndef USERVIEWER_H
#define USERVIEWER_H

#include <QWidget>

namespace Ui {
class UserViewer;
}

class UserViewer : public QWidget
{
    Q_OBJECT

public:
    explicit UserViewer(QWidget *parent = 0);
    ~UserViewer();

private:
    Ui::UserViewer *ui;
};

#endif // USERVIEWER_H
