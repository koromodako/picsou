#ifndef ACCOUNTEDITOR_H
#define ACCOUNTEDITOR_H

#include <QDialog>

namespace Ui {
class AccountEditor;
}

class AccountEditor : public QDialog
{
    Q_OBJECT

public:
    virtual ~AccountEditor();
    explicit AccountEditor(QString *name,
                           QString *description,
                           QWidget *parent = 0);

public slots:
    void accept();

private:
    QString *_name;
    QString *_description;
    Ui::AccountEditor *ui;
};

#endif // ACCOUNTEDITOR_H
