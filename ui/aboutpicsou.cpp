#include "aboutpicsou.h"
#include "ui_aboutpicsou.h"

AboutPicsou::~AboutPicsou()
{
    delete ui;
}

AboutPicsou::AboutPicsou(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPicsou)
{
    ui->setupUi(this);
//#warning TODO
    /*ui->version->setText(PICSOU_VERSION);*/
    /*ui->build->setText(PICSOU_LATEST_COMMIT);*/
    /*ui->license->setHtml(PICSOU_LICENSE);*/

    /* initialize connections */
    connect(ui->buttons, &QDialogButtonBox::accepted, this, &QWidget::close);
    connect(ui->buttons, &QDialogButtonBox::rejected, this, &QWidget::close);
}

