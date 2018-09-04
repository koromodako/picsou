#include "converter.h"
#include "utils/macro.h"

#include <QList>
#include <QPair>
#include <QDebug>
#include <QJsonObject>
#include "../picsoudb.h"

typedef void (*db_converter_t)(QJsonDocument *doc);

void convert_100_110(QJsonDocument *doc)
{
    QJsonArray user_ary, account_ary, new_user_ary, new_accout_ary;
    QJsonObject db, user, account;
    QJsonArray::iterator user_it, account_it;
    db=doc->object();
    user_ary=db[KW_DB_USERS].toArray();
    for(user_it=user_ary.begin();user_it!=user_ary.end();user_it++) {
        user=user_it->toObject();
        account_ary=user[KW_USR_ACCOUNTS].toArray();
        for(account_it=account_ary.begin();account_it!=account_ary.end();account_it++) {
            account=account_it->toObject();
            account[KW_ACT_NOTES]="";
            new_accout_ary.append(account);
        }
        user[KW_USR_ACCOUNTS]=new_accout_ary;
        new_user_ary.append(user);
    }
    db[KW_DB_VERSION]=SemVer(1,1,0).to_str();
    db[KW_DB_USERS]=new_user_ary;
    doc->setObject(db);
}

bool Converter::convert(QJsonDocument *doc, SemVer from)
{
    /* initialize convesion list */
    bool success;
    QList<QPair<SemVer, db_converter_t>>::iterator it;
    QList<QPair<SemVer, db_converter_t>> converter_list;
    converter_list.append(QPair<SemVer, db_converter_t>(SemVer(1, 0, 0), convert_100_110));
    /* apply conversions */
    LOG_DEBUG("attempting conversion.");
    for(it=converter_list.begin();it!=converter_list.end();it++) {
        LOG_DEBUG(it->first.to_str() << " == " << from.to_str() << " : " << (it->first==from));
        if(it->first==from) {
            while(it!=converter_list.end()) {
                LOG_DEBUG("applying converter for version: " << it->first.to_str());
                it->second(doc);
                it++;
            }
            success=true; goto end;
        }
    }
    success=false;
end:
    return success;
}
