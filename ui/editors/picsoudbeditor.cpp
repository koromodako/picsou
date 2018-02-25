#include "picsoudbeditor.h"
#include "ui_picsoudbeditor.h"

PicsouDBEditor::~PicsouDBEditor()
{
    delete ui;
    /* do not delete anything else here */
}

PicsouDBEditor::PicsouDBEditor(QString *name,
                               QString *description,
                               QWidget *parent) :
    QDialog(parent),
    _name(name),
    _description(description),
    ui(new Ui::PicsouDBEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("PicsouDB Editor"));

    connect(ui->save, &QPushButton::clicked,
            this, &PicsouDBEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &PicsouDBEditor::reject);
}

void PicsouDBEditor::accept()
{
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
