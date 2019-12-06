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
#include "searchfilterform.h"
#include "ui_searchfilterform.h"
#include "utils/macro.h"
#include "app/picsouuiservice.h"

SearchFilterForm::~SearchFilterForm()
{
    delete ui;
}

SearchFilterForm::SearchFilterForm(PicsouUIServicePtr ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::SearchFilterForm)
{
    ui->setupUi(this);
    /* UI initialization */
    ui->min_amount->setPrefix(tr("$"));
    ui->min_amount->setSuffix(tr(" "));
    ui->min_amount->setMinimum(0);

    ui->max_amount->setPrefix(tr("$"));
    ui->max_amount->setSuffix(tr(" "));
    ui->max_amount->setValue(100);

    ui->from->setDate(QDate::currentDate().addDays(-30));
    ui->until->setDate(QDate::currentDate());
    /* connections */
    connect(ui->search, &QPushButton::clicked, this, &SearchFilterForm::search_request);
    connect(ui->user, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_account_cb);
    connect(ui->user, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_budgets_list);
    connect(ui->account, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_pms_list);
    connect(ui->min_amount, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &SearchFilterForm::limit_max_amount);
}

SearchQuery SearchFilterForm::query() const
{
    QStringList selected_budgets, selected_pms;
    for(auto *item : ui->budgets->selectedItems()) {
        selected_budgets<<item->text();
    }
    for(auto *item : ui->pms->selectedItems()) {
        selected_pms<<item->text();
    }
    return SearchQuery(ui->user->currentText(),
                       ui->account->currentText(),
                       ui->from->date(),
                       ui->until->date(),
                       ui->min_amount->value(),
                       ui->max_amount->value(),
                       ui->description->text(),
                       ui->recipient->text(),
                       selected_budgets,
                       selected_pms);
}

void SearchFilterForm::refresh_user_cb()
{
    LOG_IN_VOID()
    ui->user->clear();
    if(!ui_svc()->populate_user_cb(ui->user)) {
        ui->user->clear();
        ui->search->setEnabled(false);
        LOG_CRITICAL("Failed to update user combo box.")
        emit search_update_failed(tr("Failed to update user combo box."));
    }
    LOG_VOID_RETURN()
}

void SearchFilterForm::refresh_account_cb(const QString &username)
{
    LOG_IN("username="<<username)
    if(username.isEmpty()) {
        LOG_VOID_RETURN()
    }
    ui->account->clear();
    if(!ui_svc()->populate_account_cb(username, ui->account)) {
        ui->account->clear();
        ui->search->setEnabled(false);
        LOG_CRITICAL("Failed to update account combo box.")
        emit search_update_failed(tr("Failed to update account combo box."));
    }
    LOG_VOID_RETURN()
}

void SearchFilterForm::refresh_budgets_list(const QString &username)
{
    LOG_IN("username="<<username)
    if(username.isEmpty()) {
        LOG_VOID_RETURN()
    }
    ui->budgets->clear();
    if(!ui_svc()->populate_budgets_list(username, ui->budgets)) {
        ui->budgets->clear();
        ui->search->setEnabled(false);
        LOG_CRITICAL("Failed to update budgets list.")
        emit search_update_failed(tr("Failed to update budgets list."));
    }
    if(ui->budgets->count()==0) {
        ui->search->setEnabled(false);
    }
    ui->budgets->selectAll();
    LOG_VOID_RETURN()
}

void SearchFilterForm::limit_max_amount(double minimum)
{
    ui->max_amount->setMinimum(minimum);
}

void SearchFilterForm::refresh_pms_list(const QString &account_name)
{
    LOG_IN("account_name="<<account_name)
    if(account_name.isEmpty()) {
        LOG_VOID_RETURN()
    }
    ui->pms->clear();
    if(!ui_svc()->populate_pms_list(ui->user->currentText(), account_name, ui->pms)) {
        ui->pms->clear();
        ui->search->setEnabled(false);
        LOG_CRITICAL("Failed to update payment methods list.")
        emit search_update_failed(tr("Failed to update payment methods list."));
    }
    if(ui->pms->count()==0) {
        ui->search->setEnabled(false);
    } else {
        ui->search->setEnabled(true);
    }
    ui->pms->selectAll();
    LOG_VOID_RETURN()
}
