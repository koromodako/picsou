#ifndef PICSOUTABLEWIDGET_H
#define PICSOUTABLEWIDGET_H

#include <QTableWidget>

#include "model/operationcollection.h"

class PicsouTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    virtual ~PicsouTableWidget();
    PicsouTableWidget(QWidget *parent=nullptr);

    void clear();
    void refresh(OperationCollection ops);
    QUuid current_op();

private:
    Q_DISABLE_COPY(PicsouTableWidget)
};

#endif // PICSOUTABLEWIDGET_H
