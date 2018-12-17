#include "converters.h"
#include "utils/macro.h"
#include "utils/crypto.h"
#include "model/object/picsoudb.h"

#include <QJsonArray>
#include <QJsonObject>

void convert_110_200(QJsonDocument *doc)
{
    LOG_IN("doc="<<doc);
    QJsonObject db=doc->object();
    QJsonArray user_ary=db[PicsouDB::KW_USERS].toArray(),
               new_user_ary;
    for(const auto user_ref : user_ary) {
        QJsonObject user=user_ref.toObject(),
                    new_user;
        /* keep name */
        new_user[User::KW_NAME]=user[User::KW_NAME];
        /* put budgets and accounts in another json object and encrypt it */
        User::KW_BUDGETS
        User::KW_ACCOUNTS

    }
    LOG_VOID_RETURN();
}
