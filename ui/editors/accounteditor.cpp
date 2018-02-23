#include "accounteditor.h"
#include "ui_accounteditor.h"

AccountEditor::~AccountEditor()
{
    delete ui;
}

AccountEditor::AccountEditor(QString *name,
                             QString *description,
                             QWidget *parent) :
    QDialog(parent),
    _name(name),
    _description(description),
    ui(new Ui::AccountEditor)
{
    ui->setupUi(this);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &AccountEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
            this, &AccountEditor::reject);
}

void AccountEditor::accept()
{
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}



