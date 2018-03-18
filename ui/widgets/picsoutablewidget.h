#ifndef PICSOUTABLEWIDGET_H
#define PICSOUTABLEWIDGET_H

#include <QTableWidget>

#include "model/operation.h"

class PicsouTableWidget : public QTableWidget
{
public:
    virtual ~PicsouTableWidget();
    PicsouTableWidget(QWidget *parent=nullptr);

    void clear();
    void refresh(QList<OperationPtr> ops);
    QUuid current_op();

private:
    Q_DISABLE_COPY(PicsouTableWidget)
};

#endif // PICSOUTABLEWIDGET_H
