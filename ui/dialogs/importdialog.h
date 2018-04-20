#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

#include "ui/widgets/picsoutablewidget.h"

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    virtual ~ImportDialog();
    explicit ImportDialog(OperationCollection ops,
                          QWidget *parent=nullptr);

private:
    PicsouTableWidget *_table;
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
