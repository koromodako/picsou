#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

#include <QDate>
#include <QRegularExpression>
#include "object/amount.h"
#include "object/operation.h"

class SearchQuery
{
public:
    virtual ~SearchQuery();
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
