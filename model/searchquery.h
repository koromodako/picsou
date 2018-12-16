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
#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

#include <QDate>
#include <QRegularExpression>
#include "object/amount.h"
#include "object/operation.h"

class SearchQuery
{
public:
    SearchQuery(const QString &username,
                const QString &account_name,
                const QDate &from, const QDate &to,
                const Amount &min, const Amount &max,
                const QString &description_re,
                const QString &recipient_re,
                const QStringList &budgets,
                const QStringList &pms);

    bool accepts(const OperationPtr &op) const;

    inline QString username() const { return _username; }
    inline QString account_name() const { return _account_name; }

private:
    QString _username;
    QString _account_name;
    QDate _from;
    QDate _to;
    Amount _min;
    Amount _max;
    QRegularExpression _description_re;
    QRegularExpression _recipient_re;
    QStringList _budgets;
    QStringList _pms;
};

struct SearchQueryFilter
{
    SearchQueryFilter(const SearchQuery &query) :
        _query(query)
    {

    }

    typedef bool result_type;

    bool operator()(const OperationPtr &op)
    {
        return _query.accepts(op);
    }

    SearchQuery _query;
};

#endif // SEARCHQUERY_H
