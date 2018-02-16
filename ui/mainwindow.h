#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    virtual ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
