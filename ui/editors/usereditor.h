#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QDialog>

#include "ui/picsouui.h"
#include "utils/crypto/qsecurememory.h"

namespace Ui {
class UserEditor;
}

class UserEditor : public QDialog, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~UserEditor();
    explicit UserEditor(PicsouUIService *ui_svc,
                        QString *username,
                        QSecureMemory *old_pwd,
                        QSecureMemory *new_pwd,
                        QWidget *parent = 0);

public slots:
    void accept();

private:
    QString *_username;
    QSecureMemory *_old_pwd;
    QSecureMemory *_new_pwd;
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
