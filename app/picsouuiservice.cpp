#include "picsouuiservice.h"
#include "utils/macro.h"

#include <QUrl>
#include <QDebug>
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
    delete _mw;
}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    _mw=new MainWindow(this);
}

bool PicsouUIService::initialize()
{
    connect(papp()->model_svc(), &PicsouModelService::updated,
            this, &PicsouUIService::notify_model_updated);

    return true;
}

void PicsouUIService::terminate()
{

}

bool PicsouUIService::populate_db_tree(QTreeWidget* const tree)
{
    bool success;
    int month, year, month_stop;
    QDate today=QDate::currentDate();
    UserPtrList users;
    AccountPtrList accounts;
    const PicsouDB * db;
    QTreeWidgetItem *root_itm, *user_itm, *account_itm, *year_itm, *month_itm;
    QIcon root_ico=QIcon(":/resources/material-design/svg/database.svg"),
          user_ico=QIcon(":/resources/material-design/svg/account-circle.svg"),
          account_ico=QIcon(":/resources/material-design/svg/account-card-details.svg"),
          calendar_ico=QIcon(":/resources/material-design/svg/calendar-blank.svg");

    if(!papp()->model_svc()->is_db_opened()) {
        success=false; goto end;
    }

    db=papp()->model_svc()->db();

    root_itm=new PicsouTreeItem(tree,
                                PicsouTreeItem::T_ROOT,
                                root_ico,
                                db->name(),
                                db->id());

    users=db->users(true);
    foreach(UserPtr user, users) {
        user_itm=new PicsouTreeItem(root_itm,
                                    PicsouTreeItem::T_USER,
                                    user_ico,
                                    user->name(),
                                    user->id());

        accounts=user->accounts(true);
        foreach(AccountPtr account, accounts) {
            account_itm=new PicsouTreeItem(user_itm,
                                           PicsouTreeItem::T_ACCOUNT,
                                           account_ico,
                                           account->name(),
                                           account->id());

            foreach (year, account->years()) {
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
                for(month=1; month<month_stop+1; month++) {
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

    success=true;
end:
    return success;
}

bool PicsouUIService::populate_user_cb(QComboBox * const cb)
{
    bool found;
    UserPtrList users;

    if(!papp()->model_svc()->is_db_opened()) {
        found=false;
        goto end;
    }

    users=papp()->model_svc()->db()->users();
    foreach (UserPtr user, users) {
        cb->addItem(user->name());
    }
    found=true;
end:
    return found;
}

bool PicsouUIService::populate_account_cb(const QString &username,
                                          QComboBox * const cb)
{
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach (UserPtr user, users) {
        if(user->name()==username) {
            found=true;
            foreach (AccountPtr account, user->accounts(true)) {
                cb->addItem(account->name());
            }
        }
    }
    return found;
}

bool PicsouUIService::populate_budgets_list(const QString &username,
                                            QListWidget * const list)
{
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach (UserPtr user, users) {
        if(user->name()==username) {
            found=true;
            foreach (BudgetPtr budget, user->budgets(true)) {
                list->addItem(budget->name());
            }
        }
    }
    return found;
}

bool PicsouUIService::populate_pms_list(const QString &username,
                                        const QString &account_name,
                                        QListWidget * const list)
{
    bool found=false;
    UserPtrList users=papp()->model_svc()->db()->users(true);
    foreach (UserPtr user, users) {
        if(user->name()==username) {
            foreach (AccountPtr account, user->accounts(true)) {
                if(account->name()==account_name) {
                    found=true;
                    foreach (PaymentMethodPtr pm, account->payment_methods(true)) {
                        list->addItem(pm->name());
                    }
                    break;
                }
            }
        }
    }
    return found;
}

#include <QDebug>

OperationCollection PicsouUIService::search_operations(const SearchQuery &query)
{
    QFuture<OperationPtr> future;
    OperationCollection ops;
    OperationPtrList ops_list;
    UserPtrList users=papp()->model_svc()->db()->users(true);

    foreach (UserPtr user, users) {
        if(user->name()==query.username()) {
            LOG_DEBUG("found username: " << user->name());
            foreach (AccountPtr account, user->accounts(true)) {
                if(account->name()==query.account_name()) {
                    LOG_DEBUG("found account: " << account->name());
                    ops_list=account->ops(true).list();

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
                            goto end;
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
end:
    return ops;
}

PicsouUIViewer *PicsouUIService::viewer_from_item(QTreeWidgetItem *item)
{
    PicsouUIViewer *w=nullptr;
    PicsouTreeItem *pitem;

    if(item==nullptr) {
        goto end;
    }

    pitem=static_cast<PicsouTreeItem*>(item);

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
end:
    return w;
}

void PicsouUIService::show_mainwindow()
{
    _mw->show();
}

void PicsouUIService::show_about_picsou()
{
    AboutPicsou *about=new AboutPicsou(_mw);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

void PicsouUIService::show_github_repo()
{
    QDesktopServices::openUrl(QUrl(PICSOU_URL, QUrl::StrictMode));
}

void PicsouUIService::show_statistics()
{

}

void PicsouUIService::db_new()
{
    QString filename, name, description;
    PicsouDBEditor db_editor(&name, &description, _mw);

    if(close_any_opened_db()) {
        emit svc_op_canceled(); goto end;
    }

    filename=QFileDialog::getSaveFileName(_mw,
                                          tr("Create file"),
                                          QString(),
                                          tr("Database (*.psdb)"));

    if(filename.isNull()) {
        emit svc_op_canceled(); goto end;
    }

    if(db_editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    if(papp()->model_svc()->new_db(filename, name, description)) {
        emit db_opened(); goto end;
    }

    emit svc_op_failed(tr("Failed to create a new database."));
end:
    return;
}

void PicsouUIService::db_open()
{
    QString filename;

    if(close_any_opened_db()) {
        emit svc_op_canceled(); goto end;
    }

    filename=QFileDialog::getOpenFileName(_mw,
                                          tr("Open file"),
                                          QString(),
                                          tr("Database (*.psdb)"));

    if(filename.isNull()) {
        emit svc_op_canceled(); goto end;
    }

    if(papp()->model_svc()->open_db(filename)) {
        emit db_opened(); goto end;
    }

    emit svc_op_failed(tr("Failed to open an existing database."));
end:
    return;
}

void PicsouUIService::db_close()
{
    if(!papp()->model_svc()->is_db_opened()) {
        goto end;
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
        emit db_closed(); goto end;
    }

    emit svc_op_failed(tr("Failed to close the database properly."));
end:
    return;
}

void PicsouUIService::db_save()
{
    if(papp()->model_svc()->save_db()) {
        emit db_saved(); goto end;
    }

    emit svc_op_failed(tr("Failed to save the database properly."));
end:
    return;
}

void PicsouUIService::db_save_as()
{
    QString filename;

    if(papp()->model_svc()->is_db_opened()) {
        filename=QFileDialog::getSaveFileName(_mw,
                                              tr("Open file"),
                                              QString(),
                                              tr("Database (*.psdb)"));

        if(filename.isNull()) {
            emit svc_op_canceled(); goto end;
        }

        if(papp()->model_svc()->save_db_as(filename)) {
            emit db_saved(); goto end;
        }
    }

    emit svc_op_failed(tr("Failed to save database in specified file."));
end:
    return;
}

void PicsouUIService::user_add()
{
    QString username;
    QSecureMemory old_pwd, new_pwd;
    UserEditor editor(&username, &old_pwd, &new_pwd, _mw);

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    papp()->model_svc()->db()->add_user(username);

    emit user_added();
end:
    return;
}

void PicsouUIService::user_edit(QUuid id)
{
    UserPtr user;
    QString username;
    QSecureMemory old_pwd, new_pwd;

    user=papp()->model_svc()->db()->find_user(id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    username=user->name();

    if(UserEditor(&username, &old_pwd, &new_pwd, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    user->update(username);

    emit user_edited();
end:
    return;
}

void PicsouUIService::user_remove(QUuid id)
{
    if(papp()->model_svc()->db()->remove_user(id)) {
        emit user_removed(); goto end;
    }

    emit svc_op_failed(tr("Failed to remove user from database."));
end:
    return;
}

void PicsouUIService::budget_add(QUuid user_id)
{
    UserPtr user;
    double amount=0.;
    QString name, description;
    BudgetEditor editor(&amount, &name, &description, _mw);


    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    user->add_budget(amount, name, description);

    emit budget_added();
end:
    return;
}

void PicsouUIService::budget_edit(QUuid user_id, QUuid budget_id)
{
    UserPtr user;
    double amount;
    BudgetPtr budget;
    QString name, description;

    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    budget=user->find_budget(budget_id);
    if(budget.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid budget pointer."));
        goto end;
    }

    amount=budget->amount();
    name=budget->name();
    description=budget->description();

    if(BudgetEditor(&amount, &name, &description, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    budget->update(amount, name, description);

    emit budget_edited();
end:
    return;
}

void PicsouUIService::budget_remove(QUuid user_id, QUuid budget_id)
{
    UserPtr user;

    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    if(user->remove_budget(budget_id)) {
        emit budget_removed(); goto end;
    }

    emit svc_op_failed(tr("Failed to remove budget from database."));
end:
    return;
}

void PicsouUIService::account_add(QUuid user_id)
{
    UserPtr user;
    QString name, description;
    AccountEditor editor(&name, &description, _mw);

    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    user->add_account(name, description);

    emit account_added();
end:
    return;
}

void PicsouUIService::account_edit(QUuid user_id, QUuid account_id)
{
    UserPtr user;
    AccountPtr account;
    QString name, notes;

    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    account=user->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    name=account->name();
    notes=account->notes();

    if(AccountEditor(&name, &notes, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    account->update(name, notes);

    emit account_edited();
end:
    return;
}

void PicsouUIService::account_remove(QUuid user_id, QUuid account_id)
{
    UserPtr user;

    user=papp()->model_svc()->db()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    if(user->remove_account(account_id)) {
        emit account_removed(); goto end;
    }

    emit svc_op_failed(tr("Failed to remove account from database."));
end:
    return;
}

void PicsouUIService::pm_add(QUuid account_id)
{
    QString name;
    PaymentMethodEditor editor(&name, _mw);
    AccountPtr account;

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    account->add_payment_method(name);

    emit pm_added();
end:
    return;
}

void PicsouUIService::pm_edit(QUuid account_id, QUuid pm_id)
{
    QString name;
    AccountPtr account;
    PaymentMethodPtr pm;

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    pm=account->find_payment_method(pm_id);
    name=pm->name();

    if(PaymentMethodEditor(&name, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    pm->update(name);

    emit pm_edited();
end:
    return;
}

void PicsouUIService::pm_remove(QUuid account_id, QUuid pm_id)
{
    AccountPtr account=papp()->model_svc()->db()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    if(account->remove_payment_method(pm_id)) {
        emit pm_removed(); goto end;
    }

    emit svc_op_failed(tr("Failed to remove payment method from database."));
end:
    return;
}

void PicsouUIService::op_add(QUuid user_id, QUuid account_id, int year, int month)
{
    QDate date;
    Amount amount;
    UserPtr user;
    AccountPtr account;
    QStringList budgets, payment_methods;
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

    user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        goto end;
    }

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    budgets=user->budgets_str(true);

    payment_methods=account->payment_methods_str(true);
    if(payment_methods.empty()) {
        emit svc_op_failed(tr("Logical error: make sure you have defined at "
                              "least one payment method before "
                              "adding operations."));
        goto end;
    }

    editor.set_budgets(budgets);
    editor.set_payment_methods(payment_methods);

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    account->add_operation(amount,
                           date,
                           budget,
                           recipient,
                           description,
                           payment_method);

    emit op_added();
end:
    return;
}

void PicsouUIService::op_edit(QUuid user_id, QUuid account_id, QUuid op_id, int year, int month)
{
    QDate date;
    UserPtr user;
    Amount amount;
    OperationPtr op;
    AccountPtr account;
    QString payment_method, budget, recipient, description;

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        return;
    }

    op=account->find_operation(op_id);
    if(op.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid op pointer."));
        return;
    }

    amount=op->amount();
    date=op->date();
    payment_method=op->payment_method();
    budget=op->budget();
    recipient=op->recipient();
    description=op->description();

    user=papp()->model_svc()->find_user(user_id);
    if(user.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid user pointer."));
        return;
    }

    OperationEditor editor(&date, &amount, &payment_method,
                           &budget, &recipient, &description,
                           year, month, _mw);

    editor.set_budgets(user->budgets_str(true));
    editor.set_payment_methods(account->payment_methods_str(true));

    if(editor.exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto end;
    }

    op->update(amount,
               date,
               budget,
               recipient,
               description,
               payment_method);

    emit op_edited();
end:
    return;
}

void PicsouUIService::op_remove(QUuid account_id, QUuid op_id)
{
    AccountPtr account;

    account=papp()->model_svc()->db()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    if(account->remove_operation(op_id)) {
        emit op_removed(); goto end;
    }

    emit svc_op_failed(tr("Failed to remove operation from database."));
end:
    return;
}

void PicsouUIService::ops_import(QUuid account_id)
{
    QString filename;
    AccountPtr account;
    OperationCollection ops;
    PicsouModelService::ImportExportFormat fmt;

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    filename=QFileDialog::getOpenFileName(_mw,
                                          tr("Import file"),
                                          QString(),
                                          tr("Files (*.csv *.xml *.json)"));

    if(filename.isNull()) {
        emit svc_op_canceled(); goto end;
    }

    if(filename.contains(".csv", Qt::CaseInsensitive)) {
        fmt=PicsouModelService::CSV;
    } else if(filename.contains(".xml", Qt::CaseInsensitive)) {
        fmt=PicsouModelService::XML;
    } else {
        fmt=PicsouModelService::JSON;
    }

    ops=papp()->model_svc()->load_ops(fmt, filename);

    if(ops.length()==0) {
        QMessageBox::warning(_mw, tr("Empty import"), tr("Import result is empty. Invalid or empty input file."));
        goto end;
    }

    if(ImportDialog(ops, _mw).exec()==QDialog::Rejected) {
        emit svc_op_canceled(); goto clean;
    }

    account->add_operations(ops);

    emit ops_imported();
    goto end;

clean:
    ops.clear();
end:
    return;
}

void PicsouUIService::ops_export(QUuid account_id)
{
    bool ok;
    QString filename, fmt_str;
    AccountPtr account;
    QStringList formats;
    PicsouModelService::ImportExportFormat eformat;
    QList<PicsouModelService::ImportExportFormat> eformats;

    account=papp()->model_svc()->find_account(account_id);
    if(account.isNull()) {
        emit svc_op_failed(tr("Internal error: invalid account pointer."));
        goto end;
    }

    formats << "CSV (*.csv)"
            << "XML (*.xml)"
            << "JSON (*.json)";
    eformats << PicsouModelService::CSV
             << PicsouModelService::XML
             << PicsouModelService::JSON;

    fmt_str=QInputDialog::getItem(_mw,
                                  tr("Which format?"),
                                  tr("Select output format"),
                                  formats,
                                  0,
                                  false,
                                  &ok);

    if(!ok) {
        emit svc_op_canceled(); goto end;
    }

    filename=QFileDialog::getSaveFileName(_mw,
                                          tr("Export file"),
                                          QString(),
                                          fmt_str);

    if(filename.isNull()) {
        emit svc_op_canceled(); goto end;
    }

    eformat=eformats.at(formats.indexOf(fmt_str));

    if(!papp()->model_svc()->dump_ops(eformat, filename, account->ops(true))) {
        emit svc_op_failed(tr("Internal error: failed to export operations."));
        goto end;
    }

    QMessageBox::information(_mw, tr("Export successful"), tr("Operation successfully exported to %0").arg(filename));
    emit ops_exported();
end:
    return;
}

void PicsouUIService::notify_model_updated(const PicsouDBPtr db)
{
    emit model_updated(db);
    emit db_modified();
}

bool PicsouUIService::close_any_opened_db()
{
    bool has_db;

    if(papp()->model_svc()->is_db_modified() &&
       QMessageBox::question(_mw,
                             tr("Close database"),
                             tr("Only one database can be opened at "
                                "once, do you want to close current "
                                "database?"),
                             QMessageBox::Close | QMessageBox::Cancel,
                             QMessageBox::Cancel)==QMessageBox::Cancel) {
        has_db=true; goto end;
    }

    db_close();
    has_db=false;
end:
    return has_db;
}
