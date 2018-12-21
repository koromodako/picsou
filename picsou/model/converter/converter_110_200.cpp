#include "converters.h"
#include "utils/macro.h"
#include "utils/cryptoctx.h"
#include "app/picsouuiservice.h"
#include "model/object/picsoudb.h"

#include <QJsonArray>
#include <QJsonObject>

bool convert_110_200(QJsonDocument *doc, PicsouUIServicePtr ui_svc)
{
    LOG_IN("doc="<<doc);
    QJsonObject db=doc->object();
    QJsonArray user_ary=db[PicsouDB::KW_USERS].toArray(),
               new_user_ary;
    for(const auto user_ref : user_ary) {
        QJsonObject new_user, user=user_ref.toObject();
        /* keep name */
        new_user[User::KW_NAME]=user[User::KW_NAME];
        /* put budgets and accounts in another json object and encrypt it */
        QJsonObject user_wrapped;
        user_wrapped[User::KW_BUDGETS]=user[User::KW_BUDGETS];
        user_wrapped[User::KW_ACCOUNTS]=user[User::KW_ACCOUNTS];
        QString pswd;
        if(!ui_svc->prompt_for_pswd(user[User::KW_NAME].toString(), pswd, true)) {
            LOG_CRITICAL("failed to retrieve user password.");
            LOG_BOOL_RETURN(false);
        }
        CryptoCtx wctx;
        if(!wctx.init(pswd)) {
            LOG_CRITICAL("CryptoCtx::init() operation failed.");
            LOG_BOOL_RETURN(false);
        }
        QString wdat;
        QJsonDocument wdoc(user_wrapped);
        if(!wctx.wrap(wdoc.toJson(QJsonDocument::Compact), wdat)) {
            LOG_CRITICAL("CryptoCtx::wrap() operation failed.");
            LOG_BOOL_RETURN(false);
        }
        new_user[PicsouDBO::KW_WDAT]=wdat;
        new_user[PicsouDBO::KW_WKEY]=wctx.wkey();
        new_user[PicsouDBO::KW_WSALT]=wctx.wsalt();
        new_user_ary.append(new_user);
    }
    db[PicsouDB::KW_VERSION]=SemVer(2, 0, 0).to_str();
    db[PicsouDB::KW_USERS]=new_user_ary;
    doc->setObject(db);
    LOG_BOOL_RETURN(true);
}
