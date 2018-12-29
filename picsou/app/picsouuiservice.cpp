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

#include "ui/viewers/lockedobjectviewer.h"
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
    delete m_mw;
    LOG_VOID_RETURN();
}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    LOG_IN("papp="<<papp);
    m_mw=new MainWindow(this);
    LOG_VOID_RETURN();
}

bool PicsouUIService::initialize()
{
    LOG_IN_VOID();
    connect(papp()->model_svc(), &PicsouModelService::updated, this, &PicsouUIService::notified_model_updated);
    connect(papp()->model_svc(), &PicsouModelService::unwrapped, this, &PicsouUIService::notified_model_unwrapped);
    LOG_BOOL_RETURN(true);
}

void PicsouUIService::terminate()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

#define GET_PSWD(title, label, ok) \
    QInputDialog::getText(m_mw, (title), (label), QLineEdit::Password, QString(), \
                          (ok), Qt::WindowFlags(), Qt::ImhSensitiveData)

bool PicsouUIService::prompt_for_pswd(const QString &username, QString &pswd, bool confirmed)
{
    LOG_IN("username="<<username<<",pswd,confirmed="<<BOOL2STR(confirmed));
    bool ok=false;
    QString cpswd, lpswd;
    while(lpswd.isNull()) {
        lpswd=GET_PSWD(tr("Passphrase for %0").arg(username), tr("Enter the passphrase:"), &ok);
        if(!ok) {
            LOG_CRITICAL("user refused to give a passphrase.");
            LOG_BOOL_RETURN(false);
        }
        if(lpswd.isNull()) {
            QMessageBox::warning(m_mw, tr("Passphrase cannot be empty."),
                                 tr("You must enter a non empty passphrase, or 'Cancel' the dialog."));
        }
    }
    while(confirmed&&cpswd.isNull()) {
        cpswd=GET_PSWD(tr("Passphrase confirmation for %0").arg(username),
                       tr("Confirm the passphrase:"),
                       &ok);
        if(!ok) {
            LOG_CRITICAL("user refused to confirm the passphrase.");
            LOG_BOOL_RETURN(false);
        }
        if(cpswd.isNull()) {
            QMessageBox::warning(m_mw, tr("Confirmation passphrase cannot be empty."),
                                 tr("You must enter a non empty confirmation passphrase, or 'Cancel' the dialog."));
        } else if(cpswd!=lpswd) {
            QMessageBox::warning(m_mw, tr("Confirmation passphrase differs from initial passphrase."),
                                 tr("You must enter the same passphrase in both dialogs, or 'Cancel' the dialog."));
        }
    }
    pswd=lpswd;
    LOG_BOOL_RETURN(true);
}

#undef GET_PSWD

bool PicsouUIService::populate_db_tree(QTreeWidget* const tree)
{
    LOG_IN("tree="<<tree);
    if(!papp()->model_svc()->is_db_opened()) {
        LOG_BOOL_RETURN(true);
    }

    static const QIcon root_ico=QIcon(":/resources/material-design/svg/database.svg"),
                       lock_ico=QIcon(":/resources/material-design/svg/folder-lock.svg"),
                       unlock_ico=QIcon(":/resources/material-design/svg/folder-lock-open.svg"),
                       account_ico=QIcon(":/resources/material-design/svg/account-card-details.svg"),
                       calendar_ico=QIcon(":/resources/material-design/svg/calendar-blank.svg");

    QDate today=QDate::currentDate();
    int month_stop, today_y=today.year();
    const PicsouDBShPtr db=papp()->model_svc()->db();
    QTreeWidgetItem *root_itm, *user_itm, *account_itm, *year_itm;
    root_itm=new PicsouTreeItem(tree, PicsouTreeItem::T_ROOT, root_ico, db->name(), db->id());

    for(const auto &user : db->users(true)) {

        if(user->wrapped()) {
            user_itm=new PicsouTreeItem(root_itm, PicsouTreeItem::T_USER, lock_ico, user->name(), user->id(), -1, -1, true);
            continue;
        } else {
            user_itm=new PicsouTreeItem(root_itm, PicsouTreeItem::T_USER, unlock_ico, user->name(), user->id());
        }

        AccountShPtrList accounts=user->accounts(true);
        for(const auto &account : accounts) {
            account_itm=new PicsouTreeItem(user_itm, PicsouTreeItem::T_ACCOUNT, account_ico,
                                           account->name(), account->id());

            for(int year=account->min_year(); year<=today_y; ++year) {
                year_itm=new PicsouTreeItem(account_itm, PicsouTreeItem::T_YEAR, calendar_ico,
                                            QString("%0").arg(year), account->id(), year);
                if(year==today_y) {
                    month_stop=today.month();
                } else {
                    month_stop=12;
                }
                for(int month=1; month<month_stop+1; month++) {
                    new PicsouTreeItem(year_itm, PicsouTreeItem::T_MONTH, calendar_ico,
                                       QDate(1,month,1).toString("MMMM"), account->id(), year, month);
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
    UserShPtrList users=papp()->model_svc()->db()->users();
    for(const auto &user : users) {
        if(!user->wrapped()) {
            cb->addItem(user->name());
        }
    }
    LOG_BOOL_RETURN(true);
}

bool PicsouUIService::populate_account_cb(const QString &username,
                                          QComboBox * const cb)
{
    LOG_IN("username="<<username<<",cb="<<cb);
    bool found=false;
    UserShPtrList users=papp()->model_svc()->db()->users(true);
    for(const auto &user : users) {
        if(user->name()==username) {
            found=true;
            for(const auto &account : user->accounts(true)) {
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
    UserShPtrList users=papp()->model_svc()->db()->users(true);
    for(const auto &user : users) {
        if(user->name()==username) {
            found=true;
            for(const auto &budget : user->budgets(true)) {
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
    UserShPtrList users=papp()->model_svc()->db()->users(true);
    for(const auto &user : users) {
        if(user->name()==username) {
            for(const auto &account : user->accounts(true)) {
                if(account->name()==account_name) {
                    found=true;
                    for(const auto &pm : account->payment_methods(true)) {
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
    QFuture<OperationShPtr> future;
    UserShPtr user=papp()->model_svc()->db()->find_user(query.username());
    if(!user.isNull()&&!user->wrapped()) {
        LOG_DEBUG("found username: "<<user->name());
        for(const auto &account : user->accounts(true)) {
            if(account->name()==query.account_name()) {
                LOG_DEBUG("found account: "<<account->name());
                OperationCollection ops_col=papp()->model_svc()->db()->ops(account->id());
                OperationShPtrList ops_list=ops_col.list(false);
                LOG_DEBUG("searching among "<<ops_list.length()<<" operations");
                future=QtConcurrent::filtered(ops_list.begin(), ops_list.end(), SearchQueryFilter(query));
                LOG_DEBUG("future min: "<<future.progressMinimum());
                LOG_DEBUG("future max: "<<future.progressMaximum());
                QProgressDialog progress(tr("Searching among %0 operations...").arg(ops_list.length()),
                                         tr("Abort search"),
                                         future.progressMinimum(),
                                         future.progressMaximum(),
                                         m_mw);
                progress.setWindowModality(Qt::WindowModal);
                while(!future.isFinished()) {
                    LOG_DEBUG("searching the dataset...");
                    progress.setValue(future.progressValue());
                    if (progress.wasCanceled()) {
                        future.cancel();
                        LOG_WARNING("search cancelled.");
                        return ops;
                    }
                    QThread::msleep(100);
                }
                progress.setValue(future.progressValue());
                ops=future.results();
                LOG_DEBUG("search successful! (results count: "<<ops.length()<<")");
                break;
            }
        }
    }
    if(ops.length()==0) {
        QMessageBox::information(m_mw, tr("No result"), tr("No operation matched the search query."));
    }
    LOG_DEBUG("-> ops.length="<<ops.length());
    return ops;
}

static bool budget_amount_cmp(const QPair<QString, Amount> &a, const QPair<QString, Amount> &b)
{
    return a.second<b.second;
}

QList<QStringList> PicsouUIService::compute_budgets(const OperationCollection &ops, QUuid user_id)
{
    QHash<QString, Amount> epb=ops.expense_per_budget();
    QList<QPair<QString, Amount>> sorted_epb;
    QHash<QString, Amount>::iterator epb_it=epb.begin();
    while(epb_it!=epb.end()) {
        sorted_epb.append(qMakePair(epb_it.key(), epb_it.value()));
        epb_it++;
    }

    std::sort(sorted_epb.begin(), sorted_epb.end(), budget_amount_cmp);

    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
    QList<QStringList> budgets;
    if(!user.isNull()&&!user->wrapped()) {
        QHash<QString, Amount> user_budgets;
        for(const auto &budget : user->budgets()) {
            user_budgets.insert(budget->name(), budget->amount());
        }
        for(const auto &budget : sorted_epb) {
            Amount allowed=user_budgets.value(budget.first)*ops.months();
            Amount consumed=budget.second;
            double percentage=100*abs(consumed.value()/allowed.value());
            budgets.append(QStringList()<<budget.first
                                        <<consumed.to_str(true)
                                        <<QString("%0% (max: %1)").arg(QString::number(percentage, 'f', 2),
                                                                       allowed.to_str(true)));
        }
    }
    return budgets;
}

QList<QStringList> PicsouUIService::compute_budgets(const OperationCollection &ops, const QString &username)
{
    UserShPtr user=papp()->model_svc()->db()->find_user(username);
    return compute_budgets(ops, user->id());
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
    if(pitem->wrapped()) {
        w=new LockedObjectViewer(this, pitem->mod_obj_id());
    } else {
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
    }
    /* trigger viewer content update */
    emit notify_model_updated(papp()->model_svc()->db());
    LOG_DEBUG("-> w="<<w);
    return w;
}

void PicsouUIService::show_mainwindow()
{
    LOG_IN_VOID();
    m_mw->show();
    LOG_VOID_RETURN();
}

void PicsouUIService::show_about_picsou()
{
    LOG_IN_VOID();
    AboutPicsou *about=new AboutPicsou(m_mw);
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

void PicsouUIService::show_license()
{
    LOG_IN_VOID();
    QDesktopServices::openUrl(QUrl(PICSOU_LICENSE_URL, QUrl::StrictMode));
    LOG_VOID_RETURN();
}

void PicsouUIService::unlock(QUuid id)
{
    LOG_IN_VOID();
    UserShPtr user=papp()->model_svc()->find_user(id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Failed to find user."));
        LOG_VOID_RETURN();
    }
    QString pswd;
    if(!prompt_for_pswd(user->name(), pswd)) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(!user->unwrap(pswd)) {
        emit svc_op_failed(tr("Failed to unwrap user."));
        LOG_VOID_RETURN();
    }
    emit unlocked();
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
    if(!close_any_opened_db()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getSaveFileName(m_mw, tr("Create file"), QString(), tr("Database (*.psdb)"));
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    PicsouDBEditor editor(m_mw);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(papp()->model_svc()->new_db(filename, editor.name(), editor.description())) {
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
    filename=QFileDialog::getOpenFileName(m_mw, tr("Open file"), QString(), tr("Database (*.psdb)"));
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
       QMessageBox::question(m_mw, tr("Save database"), tr("Do you want to save the database before closing it?"),
                             QMessageBox::Save|QMessageBox::Discard,
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
        QString filename=QFileDialog::getSaveFileName(m_mw, tr("Open file"), QString(), tr("Database (*.psdb)"));
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
    UserEditor editor(m_mw);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    papp()->model_svc()->db()->add_user(editor.username(), editor.new_pswd());
    emit user_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::user_edit(QUuid id)
{
    LOG_IN("id="<<id);
    UserShPtr user=papp()->model_svc()->db()->find_user(id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    UserEditor editor(m_mw, user->name());
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    if(!user->update(editor.username(), editor.old_pswd(), editor.new_pswd())) {
        emit svc_op_failed(tr("Internal error: failed to update."));
        LOG_VOID_RETURN();
    }
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
    UserShPtr user;
    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    BudgetEditor editor(m_mw);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    user->add_budget(editor.amount(), editor.name(), editor.description());
    emit budget_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::budget_edit(QUuid user_id, QUuid budget_id)
{
    LOG_IN("user_id="<<user_id<<",budget_id="<<budget_id);
    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    BudgetShPtr budget=user->find_budget(budget_id);
    if(budget.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid budget pointer."));
        LOG_VOID_RETURN();
    }
    BudgetEditor editor(m_mw, budget->amount(), budget->name(), budget->description());
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    budget->update(editor.amount(), editor.name(), editor.description());
    emit budget_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::budget_remove(QUuid user_id, QUuid budget_id)
{
    LOG_IN("user_id="<<user_id<<",budget_id="<<budget_id);
    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
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
    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    AccountEditor editor(m_mw);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    user->add_account(editor.name(), editor.notes());
    emit account_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::account_edit(QUuid user_id, QUuid account_id)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id);
    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountShPtr account=user->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString name=account->name();
    QString notes=account->notes();
    AccountEditor editor(m_mw, account->name(), account->notes());
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->update(editor.name(), editor.notes());
    emit account_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::account_remove(QUuid user_id, QUuid account_id)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id);
    UserShPtr user=papp()->model_svc()->db()->find_user(user_id);
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
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    PaymentMethodEditor editor(m_mw);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->add_payment_method(editor.name());
    emit pm_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::pm_edit(QUuid account_id, QUuid pm_id)
{
    LOG_IN("account_id="<<account_id<<",pm_id="<<pm_id);
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    PaymentMethodShPtr pm=account->find_payment_method(pm_id);
    if(pm.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid payment method pointer."));
        LOG_VOID_RETURN();
    }
    PaymentMethodEditor editor(m_mw, pm->name());
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    pm->update(editor.name());
    emit pm_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::pm_remove(QUuid account_id, QUuid pm_id)
{
    LOG_IN("account_id="<<account_id<<",pm_id="<<pm_id);
    AccountShPtr account=papp()->model_svc()->db()->find_account(account_id);
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

void PicsouUIService::sop_add(QUuid user_id, QUuid account_id)
{
    LOG_IN("user_id="<<user_id<<"account_id="<<account_id);
    UserShPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList budgets=user->budgets_str(true);
    if(budgets.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one budget before adding operations."));
        LOG_VOID_RETURN();
    }
    QStringList payment_methods=account->payment_methods_str(true);
    if(payment_methods.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one payment method before adding operations."));
        LOG_VOID_RETURN();
    }
    ScheduledOperationEditor editor(m_mw);
    editor.set_budgets(budgets);
    editor.set_payment_methods(payment_methods);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->add_scheduled_operation(editor.amount(),
                                     editor.budget(),
                                     editor.recipient(),
                                     editor.description(),
                                     editor.payment_method(),
                                     editor.name(),
                                     editor.schedule());
    emit sop_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::sop_edit(QUuid user_id, QUuid account_id, QUuid sop_id)
{
    LOG_IN("user_id="<<user_id<<"account_id="<<account_id<<",sop_id="<<sop_id);
    UserShPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList budgets=user->budgets_str(true);
    if(budgets.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one budget before adding operations."));
        LOG_VOID_RETURN();
    }
    QStringList payment_methods=account->payment_methods_str(true);
    if(payment_methods.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one payment method before adding operations."));
        LOG_VOID_RETURN();
    }
    ScheduledOperationShPtr sop=account->find_scheduled_operation(sop_id);
    if(sop.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid scheduled operation pointer."));
        LOG_VOID_RETURN();
    }
    ScheduledOperationEditor editor(m_mw,
                                    sop->amount(),
                                    sop->budget(),
                                    sop->recipient(),
                                    sop->description(),
                                    sop->payment_method(),
                                    sop->name(),
                                    sop->schedule());
    editor.set_budgets(budgets);
    editor.set_payment_methods(payment_methods);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    sop->update(editor.amount(),
                editor.budget(),
                editor.recipient(),
                editor.description(),
                editor.payment_method(),
                editor.name(),
                editor.schedule());
    emit sop_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::sop_remove(QUuid account_id, QUuid sop_id)
{
    LOG_IN("account_id="<<account_id<<",sop_id="<<sop_id);
    AccountShPtr account=papp()->model_svc()->db()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    if(account->remove_scheduled_operation(sop_id)) {
        emit sop_removed();
        LOG_VOID_RETURN();
    }
    emit svc_op_failed(tr("Failed to remove payment method from database."));
    LOG_VOID_RETURN();
}

void PicsouUIService::op_add(QUuid user_id, QUuid account_id, int year, int month)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id<<",year="<<year<<",month="<<month);
    UserShPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList budgets=user->budgets_str(true);
    if(budgets.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one budget before adding operations."));
        LOG_VOID_RETURN();
    }
    QStringList payment_methods=account->payment_methods_str(true);
    if(payment_methods.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at least one payment method before adding operations."));
        LOG_VOID_RETURN();
    }
    OperationEditor editor(m_mw, year, month);
    editor.set_budgets(budgets);
    editor.set_payment_methods(payment_methods);
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    account->add_operation(editor.amount(),
                           editor.date(),
                           editor.budget(),
                           editor.recipient(),
                           editor.description(),
                           editor.payment_method());
    emit op_added();
    LOG_VOID_RETURN();
}

void PicsouUIService::op_edit(QUuid user_id, QUuid account_id, QUuid op_id, int year, int month)
{
    LOG_IN("user_id="<<user_id<<",account_id="<<account_id<<",op_id="<<op_id<<",year="<<year<<",month="<<month);
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    OperationShPtr op=account->find_operation(op_id);
    if(op.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid op pointer."));
        LOG_VOID_RETURN();
    }
    UserShPtr user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        LOG_VOID_RETURN();
    }
    OperationEditor editor(m_mw, year, month,
                           op->date(),
                           op->amount(),
                           op->payment_method(),
                           op->budget(),
                           op->recipient(),
                           op->description());
    editor.set_budgets(user->budgets_str(true));
    editor.set_payment_methods(account->payment_methods_str(true));
    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    op->update(editor.amount(),
               editor.date(),
               editor.budget(),
               editor.recipient(),
               editor.description(),
               editor.payment_method());
    emit op_edited();
    LOG_VOID_RETURN();
}

void PicsouUIService::op_remove(QUuid account_id, QUuid op_id)
{
    LOG_IN("account_id="<<account_id<<",op_id="<<op_id);
    AccountShPtr account=papp()->model_svc()->db()->find_account(account_id);
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
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getOpenFileName(m_mw, tr("Import file"), QString(), tr("Files (*.csv *.xml *.json)"));
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
        QMessageBox::warning(m_mw, tr("Empty import"), tr("Import result is empty. Invalid or empty input file."));
        LOG_VOID_RETURN();
    }
    if(ImportDialog(m_mw, ops).exec()==QDialog::Rejected) {
        emit svc_op_canceled();
        ops.clear();
        LOG_VOID_RETURN();
    }
    account->add_operations(ops.list(false));
    emit ops_imported();
    LOG_VOID_RETURN();
}

void PicsouUIService::ops_export(QUuid account_id)
{
    LOG_IN("account_id="<<account_id);
    AccountShPtr account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        LOG_VOID_RETURN();
    }
    QStringList formats;
    formats<<"CSV (*.csv)"<<"XML (*.xml)"<<"JSON (*.json)";
    QList<PicsouModelService::ImportExportFormat> eformats;
    eformats<<PicsouModelService::CSV<<PicsouModelService::XML<<PicsouModelService::JSON;
    bool ok;
    QString fmt_str=QInputDialog::getItem(m_mw, tr("Which format?"), tr("Select output format"), formats, 0, false, &ok);
    if(!ok) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    QString filename=QFileDialog::getSaveFileName(m_mw, tr("Export file"), QString(), fmt_str);
    if(filename.isNull()) {
        emit svc_op_canceled();
        LOG_VOID_RETURN();
    }
    OperationCollection ops=papp()->model_svc()->db()->ops(account_id);
    PicsouModelService::ImportExportFormat eformat=eformats.at(formats.indexOf(fmt_str));
    if(!papp()->model_svc()->dump_ops(eformat, filename, ops.list(true))) {
        emit svc_op_failed(tr("Internal error: failed to export operations."));
        LOG_VOID_RETURN();
    }
    QMessageBox::information(m_mw, tr("Export successful"),
                             tr("Operation successfully exported to %0").arg(filename));
    emit ops_exported();
    LOG_VOID_RETURN();
}

void PicsouUIService::notified_model_updated(const PicsouDBShPtr db)
{
    LOG_IN_VOID();
    emit notify_model_updated(db);
    emit db_modified();
    LOG_VOID_RETURN();
}

void PicsouUIService::notified_model_unwrapped(const PicsouDBShPtr db)
{
    LOG_IN_VOID();
    emit notify_model_unwrapped(db);
    emit db_unwrapped();
    LOG_VOID_RETURN();
}

bool PicsouUIService::close_any_opened_db()
{
    LOG_IN_VOID();
    if(papp()->model_svc()->is_db_modified() &&
       QMessageBox::question(m_mw, tr("Close database"),
                             tr("Only one database can be opened at once, do you want to close current database?"),
                             QMessageBox::Close|QMessageBox::Cancel,
                             QMessageBox::Cancel)==QMessageBox::Cancel) {
        LOG_BOOL_RETURN(false);
    }
    db_close();
    LOG_BOOL_RETURN(true);
}
