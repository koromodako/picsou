#include "picsouuiservice.h"

#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QDesktopServices>

#include "picsou.h"

#include "ui/mainwindow.h"
#include "ui/aboutpicsou.h"
#include "ui/picsoudbeditor.h"

#include "model/picsoudb.h"

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
    QList<User> users;
    QList<Account> accounts;
    const PicsouDB * db;
    QTreeWidgetItem *root_itm, *user_itm, *account_itm, *year_itm, *month_itm;

    if(!papp()->model_svc()->is_db_opened()) {
        success=false; goto end;
    }

    db=papp()->model_svc()->db();

    root_itm=new QTreeWidgetItem(tree);
    root_itm->setText(0, db->name());

    users=db->users(true);
    foreach(User user, users) {
        user_itm=new QTreeWidgetItem(root_itm);
        user_itm->setText(0, user.name());

        accounts=user.accounts(true);
        foreach(Account account, accounts) {
            account_itm=new QTreeWidgetItem(user_itm);
            account_itm->setText(0, account.name());

            foreach (int year, account.years()) {
                year_itm=new QTreeWidgetItem(account_itm);
                year_itm->setText(0, QString("%0").arg(year));

                for(int month=1; month<13; month++) {
                    month_itm=new QTreeWidgetItem(year_itm);
                    month_itm->setText(0, QString("%0").arg(month));
                }
            }
        }
    }

    success=true;
end:
    return success;
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
