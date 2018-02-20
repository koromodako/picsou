#ifndef PICSOUDBEDITOR_H
#define PICSOUDBEDITOR_H

#include <QDialog>

namespace Ui {
class PicsouDBEditor;
}

class PicsouDBEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PicsouDBEditor(QString *name,
                            QString *description,
                            QWidget *parent = 0);
    ~PicsouDBEditor();

public slots:
    void accept();

private:
    QString *_name;
    QString *_description;
    Ui::PicsouDBEditor *ui;
};

#endif // PICSOUDBEDITOR_H
