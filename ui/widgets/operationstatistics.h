#ifndef OPERATIONSTATISTICS_H
#define OPERATIONSTATISTICS_H

#include <QWidget>

#include "model/operationcollection.h"

class QLabel; /* predecl */

namespace Ui {
class OperationStatistics;
}

class OperationStatistics : public QWidget
{
    Q_OBJECT

public:
    virtual ~OperationStatistics();
    explicit OperationStatistics(QWidget *parent=0);

    void refresh(const OperationCollection &ops);

    bool append_field(const QString &name, const QString &value);
    bool update_field(const QString &name, const QString &value);
    bool remove_field(const QString &name);

private:
    QHash<QString, QLabel*> _extra_fields;
    Ui::OperationStatistics *ui;
};

#endif // OPERATIONSTATISTICS_H
