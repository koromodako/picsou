#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QDialog>

#include "utils/crypto/qsecurememory.h"

namespace Ui {
class UserEditor;
}

class UserEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~UserEditor();
    explicit UserEditor(QString *username,
                        QSecureMemory *old_pwd,
                        QSecureMemory *new_pwd,
                        QWidget *parent=nullptr);

public slots:
    void accept();

private:
    QString *_username;
    QSecureMemory *_old_pwd;
    QSecureMemory *_new_pwd;
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
