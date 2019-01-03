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
#include "utils/macro.h"
#include "operationtablewidget.h"
#include "ui/items/picsoutableitem.h"
#include <QHeaderView>

OperationTableWidget::OperationTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setDragEnabled(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    clear();

    connect(this, &QTableWidget::itemClicked, this, &OperationTableWidget::verified_checked);
    connect(this, &QTableWidget::cellDoubleClicked, this, &OperationTableWidget::op_edit_requested);
}

void OperationTableWidget::clear()
{
    static QStringList labels=QStringList()<<tr("Date")
                                             <<tr("Description")
                                             <<tr("Recipient")
                                             <<tr("Payment Method")
                                             <<tr("Budget")
                                             <<tr("Amount")
                                             <<tr("Verified");

    QTableWidget::clear();
    verticalHeader()->hide();
    setRowCount(0);
    setColumnCount(7);
    setHorizontalHeaderLabels(labels);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

void OperationTableWidget::refresh(OperationCollection ops)
{
    static const int alpha=32;
    static const QIcon debit_icon=QIcon(":/resources/material-design/svg/trending-down.svg"),
                       credit_icon=QIcon(":/resources/material-design/svg/trending-up.svg"),
                       neutral_icon=QIcon(":/resources/material-design/svg/trending-neutral.svg"),
                       scheduled_icon=QIcon(":/resources/material-design/svg/calendar.svg");
    static const QColor debit_color=QColor(5, 5, 5, alpha),
                        credit_color=QColor(0, 255, 0, alpha);

    clear();
    setRowCount(ops.length());

    int r=0, c;
    QIcon icon;
    QColor bgcolor;
    QList<QTableWidgetItem*> items;
    OperationShPtrList ops_list=ops.list();
    LOG_DEBUG("adding operations...");
    for(const auto &op : ops_list) {
        LOG_DEBUG("adding op: "<<op);
        items.clear();
        switch (op->type()) {
        case Operation::NEUTRAL:
            icon=neutral_icon;
            bgcolor=Qt::white;
            break;
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
                                         op->date(),
                                         op->id(),
                                         op->scheduled()?PicsouTableItem::SCHEDULED:PicsouTableItem::NORMAL));
        items.append(new QTableWidgetItem(op->description()));
        items.append(new QTableWidgetItem(op->recipient()));
        items.append(new QTableWidgetItem(op->payment_method()));
        items.append(new QTableWidgetItem(op->budget()));
        items.append(new QTableWidgetItem(op->amount().to_str(true)));
        QTableWidgetItem *checkbox=new QTableWidgetItem(PicsouTableItem::CHECKABLE);
        if(op->scheduled()) {
            checkbox->setIcon(scheduled_icon);
        } else {
            Qt::ItemFlags flags=Qt::ItemIsUserCheckable;
            if(!m_readonly) {
                flags|=Qt::ItemIsEnabled;
            }
            checkbox->setFlags(flags);
            checkbox->setCheckState(op->verified()?Qt::Checked:Qt::Unchecked);
        }
        items.append(checkbox);
        c=0;
        for(auto *item : items) {
            item->setBackground(QBrush(bgcolor, op->scheduled()?Qt::Dense5Pattern:Qt::SolidPattern));
            setItem(r, c++, item);
        }
        r++;
    }
}

bool OperationTableWidget::is_current_op_scheduled() const
{
    QTableWidgetItem *itm=item(currentRow(), 0);
    if(itm!=nullptr) {
        return itm->type()==PicsouTableItem::SCHEDULED;
    }
    LOG_WARNING("invalid item returned!");
    return false;
}

QUuid OperationTableWidget::current_op(QTableWidgetItem *itm) const
{
    QUuid uuid;
    int row=currentRow();
    if(itm!=nullptr) {
        row=itm->row();
    }
    PicsouTableItem *it=static_cast<PicsouTableItem*>(item(row, 0));
    if(it!=nullptr) {
        uuid=it->mod_obj_id();
    }
    return uuid;
}

void OperationTableWidget::verified_checked(QTableWidgetItem *item)
{
    if(item->type()==PicsouTableItem::CHECKABLE) {
        emit op_verified_state_changed(current_op(item), item->checkState()==Qt::Checked);
    }
}
