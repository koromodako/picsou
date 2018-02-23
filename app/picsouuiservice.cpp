#include "picsouuiservice.h"

#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>

#include "picsou.h"

#include "ui/editors/picsoudbeditor.h"
#include "ui/viewers/operationviewer.h"
#include "ui/viewers/picsoudbviewer.h"
#include "ui/viewers/accountviewer.h"
#include "ui/viewers/userviewer.h"
#include "ui/items/picsoutreeitem.h"
#include "ui/mainwindow.h"
#include "ui/aboutpicsou.h"

#include "app/picsouapplication.h"
#include "app/picsoumodelservice.h"

PicsouUIService::~PicsouUIService()
{
    delete _mw;
}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    _mw = new MainWindow(this);

    connect(papp->model_svc(), &PicsouModelService::updated,
            this, &PicsouUIService::model_updated);
}

bool PicsouUIService::initialize()
{
    return true;
}

void PicsouUIService::terminate()
{

}

bool PicsouUIService::populate_db_tree(QTreeWidget* const tree)
{
    bool success;
    QList<UserPtr> users;
    QList<AccountPtr> accounts;
    const PicsouDB * db;
    QTreeWidgetItem *root_itm, *user_itm, *account_itm, *year_itm, *month_itm;

    if(!papp()->model_svc()->is_db_opened()) {
        success=false; goto end;
    }

    db=papp()->model_svc()->db();

    root_itm=new PicsouTreeItem(tree,
                                PicsouTreeItem::T_ROOT,
                                db->name(),
                                db->id());

    users=db->users(true);
    foreach(UserPtr user, users) {
        user_itm=new PicsouTreeItem(root_itm,
                                    PicsouTreeItem::T_USER,
                                    user->name(),
                                    user->id());

        accounts=user->accounts(true);
        foreach(AccountPtr account, accounts) {
            account_itm=new PicsouTreeItem(user_itm,
                                           PicsouTreeItem::T_ACCOUNT,
                                           account->name(),
                                           account->id());

            foreach (int year, account->years()) {
                year_itm=new PicsouTreeItem(account_itm,
                                            PicsouTreeItem::T_YEAR,
                                            QString("%0").arg(year),
                                            account->id());

                for(int month=0; month<12; month++) {
                    month_itm=new PicsouTreeItem(year_itm,
                                                 PicsouTreeItem::T_MONTH,
                                                 QString("%0").arg(month+1),
                                                 account->id());
                }
            }
        }
    }

    success=true;
end:
    return success;
}

QWidget *PicsouUIService::viewer_from_item(QTreeWidgetItem *item)
{
    QWidget *w=nullptr;
    PicsouTreeItem *pitem=static_cast<PicsouTreeItem*>(item);
    switch (pitem->type()) {
    case PicsouTreeItem::T_ROOT:
        w=new PicsouDBViewer(this, pitem->mod_obj_id());
        static_cast<PicsouDBViewer*>(w)->refresh(papp()->model_svc()->db());
        break;
    case PicsouTreeItem::T_USER:
        w=new UserViewer(this, pitem->mod_obj_id());
        static_cast<UserViewer*>(w)->refresh(papp()->model_svc()->db());
        break;
    case PicsouTreeItem::T_ACCOUNT:
        w=new AccountViewer(this, pitem->mod_obj_id());
        static_cast<AccountViewer*>(w)->refresh(papp()->model_svc()->db());
        break;
    case PicsouTreeItem::T_YEAR:
        w=new OperationViewer(this,
                              pitem->mod_obj_id(),
                              OperationViewer::VS_YEAR,
                              pitem->text(0).toInt());
        static_cast<OperationViewer*>(w)->refresh(papp()->model_svc()->db());
        break;
    case PicsouTreeItem::T_MONTH:
        w=new OperationViewer(this,
                              pitem->mod_obj_id(),
                              OperationViewer::VS_MONTH,
                              pitem->text(0).toInt());
        static_cast<OperationViewer*>(w)->refresh(papp()->model_svc()->db());
        break;
    }
    return w;
}

void PicsouUIService::show_mainwindow()
{
    _mw->show();
}

void PicsouUIService::show_about_picsou()
{
    AboutPicsou *about = new AboutPicsou(_mw);
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

    if(has_opened_db()) {
        emit op_canceled(); goto end;
    }

    filename=QFileDialog::getOpenFileName(_mw,
                                          tr("Create file"),
                                          QString(),
                                          tr("Database (*.psdb)"));

    if(filename.isNull()) {
        emit op_canceled(); goto end;
    }

    if(db_editor.exec()==QDialog::Rejected) {
        emit op_canceled(); goto end;
    }

    if(papp()->model_svc()->new_db(filename, name, description)) {
        emit db_opened(); goto end;
    }

    emit op_failed(tr("Failed to create a new database."));
end:
    return;
}

void PicsouUIService::db_open()
{
    QString filename;

    if(has_opened_db()) {
        emit op_canceled(); goto end;
    }

    filename = QFileDialog::getSaveFileName(_mw,
                                            tr("Open file"),
                                            QString(),
                                            tr("Database (*.psdb)"));

    if(filename.isNull()) {
        emit op_canceled(); goto end;
    }

    if(papp()->model_svc()->open_db(filename)) {
        emit db_opened(); goto end;
    }

    emit op_failed(tr("Failed to open an existing database."));
end:
    return;
}

void PicsouUIService::db_close()
{
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

    emit op_failed(tr("Failed to close the database properly."));
end:
    return;
}

void PicsouUIService::db_save()
{
    if(papp()->model_svc()->save_db()) {
        emit db_saved(); goto end;
    }

    emit op_failed(tr("Failed to save the database properly."));
end:
    return;
}

void PicsouUIService::db_save_as()
{
    QString filename;

    if(!papp()->model_svc()->is_db_opened()) {
        filename = QFileDialog::getSaveFileName(_mw,
                                                tr("Open file"),
                                                QString(),
                                                tr("Database (*.psdb)"));

        if(filename.isNull()) {
            emit op_canceled(); goto end;
        }

        if(papp()->model_svc()->save_db_as(filename)) {
            emit db_saved(); goto end;
        }
    }

    emit op_failed(tr("Failed to save database in specified file."));
end:
    return;
}

void PicsouUIService::user_add()
{

}

void PicsouUIService::user_edit(QUuid id)
{

}

void PicsouUIService::user_remove(QUuid id)
{

}

void PicsouUIService::budget_add(QUuid user_id)
{

}

void PicsouUIService::budget_edit(QUuid user_id, QUuid budget_id)
{

}

void PicsouUIService::budget_remove(QUuid user_id, QUuid budget_id)
{

}

void PicsouUIService::account_add(QUuid user_id)
{

}

void PicsouUIService::account_edit(QUuid user_id, QUuid account_id)
{

}

void PicsouUIService::account_remove(QUuid user_id, QUuid account_id)
{

}

void PicsouUIService::pm_add(QUuid account_id)
{

}

void PicsouUIService::pm_edit(QUuid account_id, QUuid pm_id)
{

}

void PicsouUIService::pm_remove(QUuid account_id, QUuid pm_id)
{

}

void PicsouUIService::op_add(QUuid account_id)
{

}

void PicsouUIService::op_edit(QUuid account_id, QUuid op_id)
{

}

void PicsouUIService::op_remove(QUuid account_id, QUuid op_id)
{

}

bool PicsouUIService::has_opened_db()
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
        has_db = true;
        goto end;
    }

    db_close();
    has_db = false;
end:
    return has_db;
}
