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
    ui->min_sb->setPrefix(tr("$"));
    ui->min_sb->setSuffix(tr(" "));

    ui->max_sb->setPrefix(tr("$"));
    ui->max_sb->setSuffix(tr(" "));

    ui->to_date->setDate(QDate::currentDate());
    /* connections */
    connect(ui->search_btn, &QPushButton::clicked, this, &SearchFilterForm::search_request);
    connect(ui->user_cb, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_account_cb);
    connect(ui->user_cb, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_budgets_list);
    connect(ui->account_cb, &QComboBox::currentTextChanged, this, &SearchFilterForm::refresh_pms_list);
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
    return SearchQuery(ui->user_cb->currentText(),
                       ui->account_cb->currentText(),
                       ui->from_date->date(),
                       ui->to_date->date(),
                       ui->min_sb->value(),
                       ui->max_sb->value(),
                       ui->description_le->text(),
                       ui->recipient_le->text(),
                       selected_budgets,
                       selected_pms);
}

void SearchFilterForm::refresh_user_cb()
{
    LOG_IN_VOID();
    ui->user_cb->clear();
    if(!ui_svc()->populate_user_cb(ui->user_cb)) {
        ui->user_cb->clear();
        ui->search_btn->setEnabled(false);
        LOG_CRITICAL("Failed to update user combo box.");
        emit search_update_failed(tr("Failed to update user combo box."));
    }
    LOG_VOID_RETURN();
}

void SearchFilterForm::refresh_account_cb(const QString &username)
{
    LOG_IN("username="<<username);
    if(username.isEmpty()) {
        LOG_VOID_RETURN();
    }
    ui->account_cb->clear();
    if(!ui_svc()->populate_account_cb(username,
                                      ui->account_cb)) {
        ui->account_cb->clear();
        ui->search_btn->setEnabled(false);
        LOG_CRITICAL("Failed to update account combo box.");
        emit search_update_failed(tr("Failed to update account combo box."));
    }
    LOG_VOID_RETURN();
}

void SearchFilterForm::refresh_budgets_list(const QString &username)
{
    LOG_IN("username="<<username);
    if(username.isEmpty()) {
        LOG_VOID_RETURN();
    }
    ui->budgets->clear();
    if(!ui_svc()->populate_budgets_list(username,
                                        ui->budgets)) {
        ui->budgets->clear();
        ui->search_btn->setEnabled(false);
        LOG_CRITICAL("Failed to update budgets list.");
        emit search_update_failed(tr("Failed to update budgets list."));
    }
    if(ui->budgets->count()==0) {
        ui->search_btn->setEnabled(false);
    }
    ui->budgets->selectAll();
    LOG_VOID_RETURN();
}

void SearchFilterForm::refresh_pms_list(const QString &account_name)
{
    LOG_IN("account_name="<<account_name);
    if(account_name.isEmpty()) {
        LOG_VOID_RETURN();
    }
    ui->pms->clear();
    if(!ui_svc()->populate_pms_list(ui->user_cb->currentText(),
                                    account_name,
                                    ui->pms)) {
        ui->pms->clear();
        ui->search_btn->setEnabled(false);
        LOG_CRITICAL("Failed to update payment methods list.");
        emit search_update_failed(tr("Failed to update payment methods list."));
    }
    if(ui->pms->count()==0) {
        ui->search_btn->setEnabled(false);
    } else {
        ui->search_btn->setEnabled(true);
    }
    ui->pms->selectAll();
    LOG_VOID_RETURN();
}
