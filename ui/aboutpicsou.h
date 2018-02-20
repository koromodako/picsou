#ifndef ABOUTPICSOU_H
#define ABOUTPICSOU_H

#include <QWidget>

namespace Ui {
class AboutPicsou;
}

class AboutPicsou : public QWidget
{
    Q_OBJECT

public:
    virtual ~AboutPicsou();
    explicit AboutPicsou(QWidget *parent = 0);

private:
    Ui::AboutPicsou *ui;
};

#endif // ABOUTPICSOU_H
