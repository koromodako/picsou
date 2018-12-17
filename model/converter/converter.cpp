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
#include "converter.h"
#include "utils/macro.h"

#include <QList>
#include <QPair>
#include <QJsonObject>
#include "model/object/picsoudb.h"

typedef void (*db_converter_t)(QJsonDocument *doc);

void convert_100_110(QJsonDocument *doc)
{
    LOG_IN("doc="<<doc);
    QJsonArray user_ary, account_ary, new_user_ary, new_accout_ary;
    QJsonObject db;
    db=doc->object();
    user_ary=db[PicsouDB::KW_USERS].toArray();
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

bool Converter::convert(QJsonDocument *doc, SemVer from)
{
    LOG_IN("doc="<<doc<<",from="<<from);
    /* initialize convesion list */
    bool convert;
    QList<QPair<SemVer, db_converter_t>> converter_list;
    converter_list.append(QPair<SemVer, db_converter_t>(SemVer(1, 0, 0), convert_100_110));
    /* apply conversions */
    LOG_DEBUG("attempting conversion.");
    convert=false;
    for(const auto &converter : converter_list) {
        LOG_DEBUG(converter.first.to_str()<<" == "<<from.to_str()<<" : "<<(converter.first==from));
        if(converter.first==from) {
            convert=true;
        }
        if(convert) {
            LOG_DEBUG("applying converter for version: "<<converter.first.to_str());
            converter.second(doc);
        }
    }
    LOG_BOOL_RETURN(convert);
}
