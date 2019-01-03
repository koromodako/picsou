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
#include "utils/amount.h"
#include "object/operation.h"

class SearchQuery
{
public:
    SearchQuery(const QString &username,
                const QString &account_name,
                const QDate &from, const QDate &to,
                const Amount &min, const Amount &max,
                const QString &description_filter,
                const QString &recipient_filter,
                const QStringList &budgets,
                const QStringList &pms);

    bool accepts(const OperationShPtr &op) const;

    inline QString username() const { return m_username; }
    inline QString account_name() const { return m_account_name; }

private:
    QString m_username;
    QString m_account_name;
    QDate m_from;
    QDate m_to;
    Amount m_min;
    Amount m_max;
    QRegularExpression m_description_re;
    QRegularExpression m_recipient_re;
    QStringList m_budgets;
    QStringList m_pms;
};

struct SearchQueryFilter
{
    SearchQueryFilter(const SearchQuery &query) :
        m_query(query)
    {

    }

    typedef bool result_type;

    bool operator()(const OperationShPtr &op)
    {
        return m_query.accepts(op);
    }

    SearchQuery m_query;
};

#endif // SEARCHQUERY_H
