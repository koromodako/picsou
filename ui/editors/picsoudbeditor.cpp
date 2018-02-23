#include "picsoudbeditor.h"
#include "ui_picsoudbeditor.h"

PicsouDBEditor::PicsouDBEditor(QString *name,
                               QString *description,
                               QWidget *parent) :
    QDialog(parent),
    _name(name),
    _description(description),
    ui(new Ui::PicsouDBEditor)
{
    ui->setupUi(this);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &PicsouDBEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
            this, &PicsouDBEditor::reject);
}

PicsouDBEditor::~PicsouDBEditor()
{
    delete ui;
    /* do not delete anything else here */
}

void PicsouDBEditor::accept()
{
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
