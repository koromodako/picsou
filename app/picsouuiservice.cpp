#include "picsouuiservice.h"
#include "utils/macro.h"

#include <QUrl>
#include <QComboBox>
#include <QListWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QtConcurrent>

#include "picsou.h"

#include "ui/dialogs/scheduledoperationeditor.h"
#include "ui/dialogs/paymentmethodeditor.h"
#include "ui/dialogs/operationeditor.h"
#include "ui/dialogs/picsoudbeditor.h"
#include "ui/dialogs/accounteditor.h"
#include "ui/dialogs/budgeteditor.h"
#include "ui/dialogs/importdialog.h"
#include "ui/dialogs/aboutpicsou.h"
#include "ui/dialogs/usereditor.h"

#include "ui/viewers/operationviewer.h"
#include "ui/viewers/picsoudbviewer.h"
#include "ui/viewers/accountviewer.h"
#include "ui/viewers/userviewer.h"

#include "ui/items/picsoutreeitem.h"

#include "ui/mainwindow.h"

#include "app/picsouapplication.h"
#include "app/picsoumodelservice.h"

PicsouUIService::~PicsouUIService()
{
    LOG_IN_VOID();
    delete _mw;
    LOG_VOID_RETURN();
}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    LOG_IN("papp="<<papp);
    _mw=new MainWindow(this);
    LOG_VOID_RETURN();
}

bool PicsouUIService::initialize()
{
    LOG_IN_VOID();
    connect(papp()->model_svc(), &PicsouModelService::updated,
            this, &PicsouUIService::notify_model_updated);
    LOG_BOOL_RETURN(true);
}

void PicsouUIService::terminate()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

bool PicsouUIService::populate_db_tree(QTreeWidget* const tree)
{
    LOG_IN("tree="<<tree);
    if(!papp()->model_svc()->is_db_opened()) {
        LOG_BOOL_RETURN(true);
    }

    static const QIcon root_ico=QIcon(":/resources/material-design/svg/database.svg"),
                       user_ico=QIcon(":/resources/material-design/svg/account-circle.svg"),
                       account_ico=QIcon(":/resources/material-design/svg/account-card-details.svg"),
                       calendar_ico=QIcon(":/resources/material-design/svg/calendar-blank.svg");

    int month_stop;
    QDate today=QDate::currentDate();
    const PicsouDB *db=papp()->model_svc()->db();
    QTreeWidgetItem *root_itm, *user_itm, *account_itm, *year_itm, *month_itm;
    root_itm=new PicsouTreeItem(tree,
                                PicsouTreeItem::T_ROOT,
                                root_ico,
                                db->name(),
                                db->id());

    foreach(UserPtr user, db->users(true)) {
        user_itm=new PicsouTreeItem(root_itm,
                                    PicsouTreeItem::T_USER,
                                    user_ico,
                                    user->name(),
                                    user->id());

        AccountPtrList accounts=user->accounts(true);
        foreach(AccountPtr account, accounts) {
            account_itm=new PicsouTreeItem(user_itm,
                                           PicsouTreeItem::T_ACCOUNT,
                                           account_ico,
                                           account->name(),
                                           account->id());

            foreach(int year, account->years()) {
                year_itm=new PicsouTreeItem(account_itm,
                                            PicsouTreeItem::T_YEAR,
                                            calendar_ico,
                                            QString("%0").arg(year),
                                            account->id(),
                                            year);
                if(year==today.year()) {
                    month_stop=today.month();
                } else {
                    month_stop=12;
                }
                for(int month=1; month<month_stop+1; month++) {
                    month_itm=new PicsouTreeItem(year_itm,
                                                 PicsouTreeItem::T_MONTH,
                                                 calendar_ico,
                                                 QDate(1,month,1).toString("MMMM"),
                                                 account->id(),
                                                 year,
                                                 month);
                }
            }
        }
    }
    LOG_BOOL_RETURN(true);
}

bool PicsouUIService::populate_user_cb(QComboBox * const cb)
{
    LOG_IN("cb="<<cb);
    if(!papp()->model_svc()->is_db_opened()) {
        LOG_BOOL_RETURN(false);
    }
    UserPtrList users=papp()->model_svc()->db()->users();
    foreach(UserPtr user, users) {
        cb->addItem(user->name());
    }
    LOG_BOOL_RETURN(true);
}

bool PicsouUIService::populate_account_cb(const QString &username,
                                          QComboBox * const cb)
{
    LOG_IN("username="<<username<<",cb="<<cb);
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach(UserPtr user, users) {
        if(user->name()==username) {
            found=true;
            foreach(AccountPtr account, user->accounts(true)) {
                cb->addItem(account->name());
            }
        }
    }
    LOG_BOOL_RETURN(found);
}

bool PicsouUIService::populate_budgets_list(const QString &username,
                                            QListWidget * const list)
{
    LOG_IN("username="<<username<<",list="<<list);
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach(UserPtr user, users) {
        if(user->name()==username) {
            found=true;
            foreach(BudgetPtr budget, user->budgets(true)) {
                list->addItem(budget->name());
            }
        }
    }
    LOG_BOOL_RETURN(found);
}

bool PicsouUIService::populate_pms_list(const QString &username,
                                        const QString &account_name,
                                        QListWidget * const list)
{
    LOG_IN("username="<<username<<",account_name="<<account_name<<",list="<<list);
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach(UserPtr user, users) {
        if(user->name()==username) {
            foreach(AccountPtr account, user->accounts(true)) {
                if(account->name()==account_name) {
                    found=true;
                    foreach(PaymentMethodPtr pm, account->payment_methods(true)) {
                        list->addItem(pm->name());
                    }
                    break;
                }
            }
        }
    }
    LOG_BOOL_RETURN(found);
}

OperationCollection PicsouUIService::search_operations(const SearchQuery &query)
{
    LOG_IN("query="<<&query);
    OperationCollection ops;
    QFuture<OperationPtr> future;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach(UserPtr user, users) {
        if(user->name()==query.username()) {
            LOG_DEBUG("found username: " << user->name());
            foreach(AccountPtr account, user->accounts(true)) {
                if(account->name()==query.account_name()) {
                    LOG_DEBUG("found account: " << account->name());
                    OperationPtrList ops_list=account->ops(true).list();

                    LOG_DEBUG("searching among " << ops_list.length() << " operations");
                    future=QtConcurrent::filtered(ops_list.begin(), ops_list.end(), SearchQueryFilter(query));

                    LOG_DEBUG("future min: " << future.progressMinimum());
                    LOG_DEBUG("future max: " << future.progressMaximum());

                    QProgressDialog progress(tr("Searching among %0 operations...").arg(ops_list.length()),
                                             tr("Abort search"),
                                             future.progressMinimum(),
                                             future.progressMaximum(),
                                             _mw);
                    progress.setWindowModality(Qt::WindowModal);

                    while(!future.isFinished()) {
                        LOG_DEBUG("searching the dataset...");
                        progress.setValue(future.progressValue());
                        if (progress.wasCanceled()) {
                            future.cancel();
                            LOG_WARNING("search cancelled.")
                            return ops;
                        }
                        QThread::msleep(100);
                    }
                    progress.setValue(future.progressValue());

                    ops=future.results();
                    LOG_DEBUG("search successful! (results count: " << ops.length() << ")");

                    break;
                }
            }
        }
    }
    if(ops.length()==0) {
        QMessageBox::information(_mw,
                                 tr("No result"),
                                 tr("No operation matched the search query."),
                                 QMessageBox::Ok);
    }
    LOG_INFO("-> ops.length="<<ops.length());
    return ops;
}

PicsouUIViewer *PicsouUIService::viewer_from_item(QTreeWidgetItem *item)
{
    LOG_IN("item="<<item);
    PicsouUIViewer *w=nullptr;
    if(item==nullptr) {
        LOG_WARNING("tree widget item is null.");
        return w;
    }
    PicsouTreeItem *pitem=static_cast<PicsouTreeItem*>(item);
    switch (pitem->type()) {
    case PicsouTreeItem::T_ROOT:
        w=new PicsouDBViewer(this, pitem->mod_obj_id());
        break;
    case PicsouTreeItem::T_USER:
        w=new UserViewer(this, pitem->mod_obj_id());
        break;
    case PicsouTreeItem::T_ACCOUNT:
        w=new AccountViewer(this,
                            pitem->parent()->mod_obj_id(),
                            pitem->mod_obj_id());
        break;
    case PicsouTreeItem::T_YEAR:
        w=new OperationViewer(this,
                              pitem->parent()->parent()->mod_obj_id(),
                              pitem->parent()->mod_obj_id(),
                              OperationViewer::VS_YEAR,
                              pitem->year());
        break;
    case PicsouTreeItem::T_MONTH:
        w=new OperationViewer(this,
                              pitem->parent()->parent()->parent()->mod_obj_id(),
                              pitem->parent()->parent()->mod_obj_id(),
                              OperationViewer::VS_MONTH,
                              pitem->year(),
                              pitem->month());
        break;
    }
    /* trigger viewer content update */
    emit model_updated(papp()->model_svc()->db());
    LOG_INFO("w="<<w);
    return w;
}

void PicsouUIService::show_mainwindow()
{
    LOG_IN_VOID();
    _mw->show();
    LOG_VOID_RETURN();
}

void PicsouUIService::show_about_picsou()
{
    LOG_IN_VOID();
    AboutPicsou *about=new AboutPicsou(_mw);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
    LOG_VOID_RETURN();
}

void PicsouUIService::show_github_repo()
{
    LOG_IN_VOID();
    QDesktopServices::openUrl(QUrl(PICSOU_URL, QUrl::StrictMode));
    LOG_VOID_RETURN();
}

void PicsouUIService::show_statistics()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

void PicsouUIService::db_new()
{
    LOG_IN_VOID();
    QString name, description;
    PicsouDBEditor db_editor(&name, &description, _mw);
    if(!close_any_opened_db()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getSaveFileName(_mw,
                                          tr("Create file"),
                                          QString(),
                                          tr("Database (*.psdb)"));
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(db_editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(papp()->model_svc()->new_db(filename, name, description)) {
        emit db_opened();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to create a new database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::db_open()
{
    LOG_IN_VOID();
    QString filename;
    if(!close_any_opened_db()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    filename=QFileDialog::getOpenFileName(_mw,
                                          tr("Open file"),
                                          QString(),
                                          tr("Database (*.psdb)"));
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(papp()->model_svc()->open_db(filename)) {
        emit db_opened();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to open an existing database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::db_close()
{
    LOG_IN_VOID();
    if(!papp()->model_svc()->is_db_opened()) {
        LOG_VOID_RETURN();
    }
    if(papp()->model_svc()->is_db_modified() &&
       QMessageBox::question(_mw,
                             tr("Save database"),
                             tr("Do you want to save the database before "
                                "closing it?"),
                             QMessageBox::Save | QMessageBox::Discard,
                             QMessageBox::Save)==QMessageBox::Save) {
        db_save();
    }
    if(papp()->model_svc()->close_db()) {
        emit db_closed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to close the database properly."));
    LOG_VOID_RETURN();
}

void PicsouUIService::db_save()
{
    LOG_IN_VOID();
    if(papp()->model_svc()->save_db()) {
        emit db_saved();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to save the database properly."));
    LOG_VOID_RETURN();
}

void PicsouUIService::db_save_as()
{
    LOG_IN_VOID();
    if(papp()->model_svc()->is_db_opened()) {
        QString filename=QFileDialog::getSaveFileName(_mw,
                                              tr("Open file"),
                                              QString(),
                                              tr("Database (*.psdb)"));
        if(filename.isNull()) {
            emit svc_op_canceled();
            LOG_VOID_RETURN();
        }
        if(papp()->model_svc()->save_db_as(filename)) {
            emit db_saved();
            LOG_VOID_RETURN();
        }
    }
    emit svc_op_failed(tr("Failed to save database in specified file."));
    LOG_VOID_RETURN();
}

void PicsouUIService::user_add()
{
    LOG_IN_VOID();
    QString username;
    QSecureMemory old_pwd, new_pwd;
    if(UserEditor(&username, &old_pwd, &new_pwd, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    papp()->model_svc()->db()->add_user(username);
    emit user_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::user_edit(QUuid id)
{
    LOG_IN("id="<<id);
    QSecureMemory old_pwd, new_pwd;
    UserPtr user=papp()->model_svc()->db()->find_user(id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    QString username=user->name();
    if(UserEditor(&username, &old_pwd, &new_pwd, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    user->update(username);
    emit user_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::user_remove(QUuid id)
{
    LOG_IN("id="<<id);
    if(papp()->model_svc()->db()->remove_user(id)) {
        emit user_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove user from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::budget_add(QUuid user_id)
{
    LOG_IN("user_id="<<user_id);
    UserPtr user;
    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    double amount=0.;
    QString name, description;
    if(BudgetEditor (&amount, &name, &description, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    user->add_budget(amount, name, description);
    emit budget_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::budget_edit(QUuid user_id, QUuid budget_id)
{
    LOG_IN("user_id="<<user_id<<",budget_id="<<budget_id);
    UserPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    BudgetPtr budget=user->find_budget(budget_id);
    if(budget.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid budget pointer."));
        LOG_VOID_RETURN();
    }
    double amount=budget->amount();
    QString name=budget->name();
    QString description=budget->description();
    if(BudgetEditor(&amount, &name, &description, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    budget->update(amount, name, description);
    emit budget_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::budget_remove(QUuid user_id, QUuid budget_id)
{
    LOG_IN("user_id="<<user_id<<",budget_id="<<budget_id);
    UserPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    if(user->remove_budget(budget_id)) {
        emit budget_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove budget from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::account_add(QUuid user_id)
{
    LOG_IN("user_id="<<user_id);
    UserPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString name, description;
    if(AccountEditor(&name, &description, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    user->add_account(name, description);
    emit account_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::account_edit(QUuid user_id, QUuid account_id)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id);
    UserPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountPtr account=user->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString name=account->name();
    QString notes=account->notes();
    if(AccountEditor(&name, &notes, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->update(name, notes);
    emit account_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::account_remove(QUuid user_id, QUuid account_id)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id);
    UserPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    if(user->remove_account(account_id)) {
        emit account_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove account from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::pm_add(QUuid account_id)
{
    LOG_IN("account_id="<<account_id);
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString name;
    if(PaymentMethodEditor(&name, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->add_payment_method(name);
    emit pm_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::pm_edit(QUuid account_id, QUuid pm_id)
{
    LOG_IN("account_id="<<account_id<<",pm_id="<<pm_id);
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    PaymentMethodPtr pm=account->find_payment_method(pm_id);
    QString name=pm->name();
    if(PaymentMethodEditor(&name, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    pm->update(name);
    emit pm_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::pm_remove(QUuid account_id, QUuid pm_id)
{
    LOG_IN("account_id="<<account_id<<",pm_id="<<pm_id);
    AccountPtr account=papp()->model_svc()->db()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    if(account->remove_payment_method(pm_id)) {
        emit pm_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove payment method from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::op_add(QUuid user_id, QUuid account_id, int year, int month)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id<<",year="<<year<<",month="<<month);

    UserPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList budgets=user->budgets_str(true);
    QStringList payment_methods=account->payment_methods_str(true);
    if(payment_methods.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at "
                              "least one payment method before "
                              "adding operations."));
        LOG_VOID_RETURN();
    }
    QDate date;
    Amount amount;
    QString payment_method, budget, recipient, description;
    OperationEditor editor(&date,
                           &amount,
                           &payment_method,
                           &budget,
                           &recipient,
                           &description,
                           year,
                           month,
                           _mw);
    editor.set_budgets(budgets);
    editor.set_payment_methods(payment_methods);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->add_operation(amount,
                           date,
                           budget,
                           recipient,
                           description,
                           payment_method);

    emit op_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::op_edit(QUuid user_id, QUuid account_id, QUuid op_id, int year, int month)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id<<",op_id="<<op_id<<",year="<<year<<",month="<<month);
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    OperationPtr op=account->find_operation(op_id);
    if(op.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid op pointer."));
        LOG_VOID_RETURN();
    }
    QDate date=op->date();
    Amount amount=op->amount();
    QString payment_method=op->payment_method(),
            budget=op->budget(),
            recipient=op->recipient(),
            description=op->description();
    UserPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    OperationEditor editor(&date, &amount, &payment_method,
                           &budget, &recipient, &description,
                           year, month, _mw);
    editor.set_budgets(user->budgets_str(true));
    editor.set_payment_methods(account->payment_methods_str(true));
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    op->update(amount,
               date,
               budget,
               recipient,
               description,
               payment_method);
    emit op_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::op_remove(QUuid account_id, QUuid op_id)
{
    LOG_IN("account_id="<<account_id<<",op_id="<<op_id);
    AccountPtr account=papp()->model_svc()->db()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    if(account->remove_operation(op_id)) {
        emit op_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove operation from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::ops_import(QUuid account_id)
{
    LOG_IN("account_id="<<account_id);
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getOpenFileName(_mw,
                                                  tr("Import file"),
                                                  QString(),
                                                  tr("Files (*.csv *.xml *.json)"));
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    PicsouModelService::ImportExportFormat fmt;
    if(filename.contains(".csv", Qt::CaseInsensitive)) {
        fmt=PicsouModelService::CSV;
    } else if(filename.contains(".xml", Qt::CaseInsensitive)) {
        fmt=PicsouModelService::XML;
    } else {
        fmt=PicsouModelService::JSON;
    }
    OperationCollection ops=papp()->model_svc()->load_ops(fmt, filename);
    if(ops.length()==0) {
        QMessageBox::warning(_mw, tr("Empty import"), tr("Import result is empty. Invalid or empty input file."));
        LOG_VOID_RETURN();
    }
    if(ImportDialog(ops, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        ops.clear();
        LOG_VOID_RETURN();
    }
    account->add_operations(ops);
    emit ops_imported();
    LOG_VOID_RETURN();
}

void PicsouUIService::ops_export(QUuid account_id)
{
    LOG_IN("account_id="<<account_id);
    AccountPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList formats;
    formats << "CSV (*.csv)"
            << "XML (*.xml)"
            << "JSON (*.json)";
    QList<PicsouModelService::ImportExportFormat> eformats;
    eformats << PicsouModelService::CSV
             << PicsouModelService::XML
             << PicsouModelService::JSON;
    bool ok;
    QString fmt_str=QInputDialog::getItem(_mw,
                                          tr("Which format?"),
                                          tr("Select output format"),
                                          formats,
                                          0,
                                          false,
                                          &ok);
    if(!ok) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getSaveFileName(_mw,
                                                  tr("Export file"),
                                                  QString(),
                                                  fmt_str);
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    PicsouModelService::ImportExportFormat eformat=eformats.at(formats.indexOf(fmt_str));
    if(!papp()->model_svc()->dump_ops(eformat, filename, account->ops(true))) {
        emit svc_op_failed(tr("Internal error: failed to export operations."));
        LOG_VOID_RETURN();
    }
    QMessageBox::information(_mw, tr("Export successful"), tr("Operation successfully exported to %0").arg(filename));
    emit ops_exported();
    LOG_VOID_RETURN();
}

void PicsouUIService::notify_model_updated(const PicsouDBPtr db)
{
    LOG_IN_VOID();
    emit model_updated(db);
    emit db_modified();
    LOG_VOID_RETURN();
}

bool PicsouUIService::close_any_opened_db()
{
    LOG_IN_VOID();
    if(papp()->model_svc()->is_db_modified() &&
       QMessageBox::question(_mw,
                             tr("Close database"),
                             tr("Only one database can be opened at "
                                "once, do you want to close current "
                                "database?"),
                             QMessageBox::Close | QMessageBox::Cancel,
                             QMessageBox::Cancel)==QMessageBox::Cancel) {
        LOG_BOOL_RETURN(false);
    }
    db_close();
    LOG_BOOL_RETURN(true);
}
