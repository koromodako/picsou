#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "picsouui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~MainWindow();
    explicit MainWindow(PicsouUIService *ui_svc, QWidget *parent = nullptr);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
