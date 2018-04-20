#include "picsoutablewidget.h"

#include "ui/items/picsoutableitem.h"

#include <QHeaderView>



PicsouTableWidget::~PicsouTableWidget()
{

}

PicsouTableWidget::PicsouTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setDragEnabled(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);

    clear();
}

void PicsouTableWidget::clear()
{
    static QStringList labels = QStringList() << tr("Date")
                                              << tr("Description")
                                              << tr("Recipient")
                                              << tr("Payment Method")
                                              << tr("Budget")
                                              << tr("Amount");

    QTableWidget::clear();
    verticalHeader()->hide();
    setColumnCount(6);
    setHorizontalHeaderLabels(labels);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

void PicsouTableWidget::refresh(OperationCollection ops)
{
    int r=0, c, alpha=64;
    QIcon icon;
    QColor bgcolor;
    QList<QTableWidgetItem *> items;

    clear();
    setRowCount(ops.length());

    foreach (OperationPtr op, ops.list()) {
        items.clear();

        switch (op->type()) {
        case Operation::DEBIT:
            icon=QIcon(":/resources/material-design/svg/trending-down.svg");
            bgcolor=QColor(255, 0, 0, alpha);
            break;
        case Operation::CREDIT:
            icon=QIcon(":/resources/material-design/svg/trending-up.svg");
            bgcolor=QColor(0, 255, 0, alpha);
            break;
        }

        items.append(new PicsouTableItem(icon,
                                         op->date().toString(Qt::DefaultLocaleShortDate),
                                         op->id()));
        items.append(new QTableWidgetItem(op->description()));
        items.append(new QTableWidgetItem(op->recipient()));
        items.append(new QTableWidgetItem(op->payment_method()));
        items.append(new QTableWidgetItem(op->budget()));
        items.append(new QTableWidgetItem(op->amount().to_str(true)));

        c=0;
        foreach (QTableWidgetItem *item, items) {
            item->setBackgroundColor(bgcolor);
            setItem(r, c++, item);
        }
        r++;
    }
}

QUuid PicsouTableWidget::current_op()
{
    QUuid uuid;
    PicsouTableItem *it=static_cast<PicsouTableItem*>(item(currentRow(), 0));
    if(it!=nullptr) {
        uuid=it->mod_obj_id();
    }
    return uuid;
}
