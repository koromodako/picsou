#ifndef OPERATIONSTATISTICS_H
#define OPERATIONSTATISTICS_H

#include <QWidget>

#include "model/operationcollection.h"

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

private:
    Ui::OperationStatistics *ui;
};

#endif // OPERATIONSTATISTICS_H
