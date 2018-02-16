#ifndef PICSOUDBVIEWER_H
#define PICSOUDBVIEWER_H

#include <QWidget>

namespace Ui {
class PicsouDBViewer;
}

class PicsouDBViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PicsouDBViewer(QWidget *parent = 0);
    ~PicsouDBViewer();

private:
    Ui::PicsouDBViewer *ui;
};

#endif // PICSOUDBVIEWER_H
