/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
    static QStringList labels=QStringList()<<tr("Date")
                                             <<tr("Description")
                                             <<tr("Recipient")
                                             <<tr("Payment Method")
                                             <<tr("Budget")
                                             <<tr("Amount");

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
    static const int alpha=32;
    static const QIcon debit_icon=QIcon(":/resources/material-design/svg/trending-down.svg"),
                       credit_icon=QIcon(":/resources/material-design/svg/trending-up.svg");
    static const QColor debit_color=QColor(5, 5, 5, alpha),
                        credit_color=QColor(0, 255, 0, alpha);

    int r=0, c;
    QIcon icon;
    QColor bgcolor;
    QList<QTableWidgetItem *> items;

    clear();
    setRowCount(ops.length());

    for(const auto &op : ops.list()) {
        items.clear();
        switch (op->type()) {
        case Operation::DEBIT:
            icon=debit_icon;
            bgcolor=debit_color;
            break;
        case Operation::CREDIT:
            icon=credit_icon;
            bgcolor=credit_color;
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
        for(auto *item : items) {
            item->setBackground(QBrush(bgcolor, op->scheduled()?Qt::DiagCrossPattern:Qt::SolidPattern));
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
