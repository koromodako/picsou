#ifndef ACCOUNTEDITOR_H
#define ACCOUNTEDITOR_H

#include <QWidget>

namespace Ui {
class AccountEditor;
}

class AccountEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AccountEditor(QWidget *parent = 0);
    ~AccountEditor();

private:
    Ui::AccountEditor *ui;
};

#endif // ACCOUNTEDITOR_H
