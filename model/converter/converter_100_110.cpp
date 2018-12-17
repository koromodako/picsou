#include "converters.h"
#include "utils/macro.h"
#include "model/object/picsoudb.h"

#include <QJsonArray>
#include <QJsonObject>

void convert_100_110(QJsonDocument *doc)
{
    LOG_IN("doc="<<doc);
    QJsonObject db=doc->object();
    QJsonArray user_ary=db[PicsouDB::KW_USERS].toArray(),
               account_ary,
               new_user_ary,
               new_accout_ary;
    for(const auto user_ref : user_ary) {
        QJsonObject user=user_ref.toObject();
        account_ary=user[User::KW_ACCOUNTS].toArray();
        for(const auto account_ref : account_ary) {
            QJsonObject account=account_ref.toObject();
            account[Account::KW_NOTES]="";
            new_accout_ary.append(account);
        }
        user[User::KW_ACCOUNTS]=new_accout_ary;
        new_user_ary.append(user);
    }
    db[PicsouDB::KW_VERSION]=SemVer(1,1,0).to_str();
    db[PicsouDB::KW_USERS]=new_user_ary;
    doc->setObject(db);
    LOG_VOID_RETURN();
}
