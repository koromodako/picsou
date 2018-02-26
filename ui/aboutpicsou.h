#ifndef ABOUTPICSOU_H
#define ABOUTPICSOU_H

#include <QDialog>

namespace Ui {
class AboutPicsou;
}

class AboutPicsou : public QDialog
{
    Q_OBJECT

public:
    virtual ~AboutPicsou();
    explicit AboutPicsou(QWidget *parent=nullptr);

private:
    Ui::AboutPicsou *ui;
};

#endif // ABOUTPICSOU_H
