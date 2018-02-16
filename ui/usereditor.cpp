#include "usereditor.h"
#include "ui_usereditor.h"

UserEditor::UserEditor(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::UserEditor)
{
    ui->setupUi(this);
}

UserEditor::~UserEditor()
{
    delete ui;
}
