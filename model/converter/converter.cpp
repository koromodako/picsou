#include "converter.h"
#include "utils/macro.h"

#include <QList>
#include <QPair>
#include <QJsonObject>
#include "../picsoudb.h"

typedef void (*db_converter_t)(QJsonDocument *doc);

void convert_100_110(QJsonDocument *doc)
{
    LOG_IN("doc="<<doc);
    QJsonArray user_ary, account_ary, new_user_ary, new_accout_ary;
    QJsonObject db;
    db=doc->object();
    user_ary=db[KW_DB_USERS].toArray();
    for(auto const user_ref : user_ary) {
        QJsonObject user=user_ref.toObject();
        account_ary=user[KW_USR_ACCOUNTS].toArray();
        for(auto const account_ref : account_ary) {
            QJsonObject account=account_ref.toObject();
            account[KW_ACT_NOTES]="";
            new_accout_ary.append(account);
        }
        user[KW_USR_ACCOUNTS]=new_accout_ary;
        new_user_ary.append(user);
    }
    db[KW_DB_VERSION]=SemVer(1,1,0).to_str();
    db[KW_DB_USERS]=new_user_ary;
    doc->setObject(db);
    LOG_VOID_RETURN();
}

bool Converter::convert(QJsonDocument *doc, SemVer from)
{
    LOG_IN("doc="<<doc<<",from="<<from);
    /* initialize convesion list */
    bool convert;
    QList<QPair<SemVer, db_converter_t>> converter_list;
    converter_list.append(QPair<SemVer, db_converter_t>(SemVer(1, 0, 0), convert_100_110));
    /* apply conversions */
    LOG_DEBUG("attempting conversion.");
    convert = false;
    for(auto it : converter_list) {
        LOG_DEBUG(it.first.to_str() << " == " << from.to_str() << " : " << (it.first==from));
        if(it.first==from) {
            convert = true;
        }
        if(convert) {
            LOG_DEBUG("applying converter for version: " << it.first.to_str());
            it.second(doc);
        }
    }
    LOG_BOOL_RETURN(convert);
}
