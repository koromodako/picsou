#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QWidget>

namespace Ui {
class UserEditor;
}

class UserEditor : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditor(QWidget *parent = 0);
    ~UserEditor();

private:
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
