#include "searchquery.h"
#include "utils/macro.h"

SearchQuery::~SearchQuery()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

SearchQuery::SearchQuery(const QString &username,
                         const QString &account_name,
                         const QDate &from,
                         const QDate &to,
                         const Amount &min,
                         const Amount &max,
                         const QString &description_re,
                         const QString &recipient_re,
                         const QStringList &budgets,
                         const QStringList &pms) :
    _username(username),
    _account_name(account_name),
    _from(from),
    _to(to),
    _min(min),
    _max(max),
    _description_re(description_re, QRegularExpression::CaseInsensitiveOption),
    _recipient_re(recipient_re, QRegularExpression::CaseInsensitiveOption),
    _budgets(budgets),
    _pms(pms)
{
    LOG_IN("username="<<username
           <<",account_name="<<account_name
           <<",from="<<from
           <<",to="<<to
           <<",min="<<min
           <<",max="<<max
           <<",description_re="<<description_re
           <<",recipient_re="<<recipient_re
           <<",budgets="<<budgets
           <<",pms="<<pms);
    _description_re.optimize();
    _recipient_re.optimize();
    LOG_VOID_RETURN();
}

bool SearchQuery::accepts(const OperationPtr &op) const
{
    LOG_IN("op="<<op);
    QDate date=op->date();
    Amount amount=qAbs(op->amount());
    QString recipient=op->recipient();
    QString description=op->description();
    if(date<_from||date>_to) {
        LOG_DEBUG("rejecting "<<op<<" because "<<date<<"<"<<_from<<"||"<<date<<">"<<_to);
        LOG_BOOL_RETURN(false);
    }
    if(amount<_min||amount>_max) {
        LOG_DEBUG("rejecting "<<op<<" because "<<amount.value()<<"<"<<_min.value()<<"||"<<amount.value()<<">"<<_max.value());
        LOG_BOOL_RETURN(false);
    }
    if(!_budgets.contains(op->budget())) {
        LOG_DEBUG("rejecting "<<op<<" because "<<op->budget()<<" not in "<<_budgets);
        LOG_BOOL_RETURN(false);
    }
    if(!_pms.contains(op->payment_method())) {
        LOG_DEBUG("rejecting "<<op<<" because "<<op->payment_method()<<" not in "<<_pms);
        LOG_BOOL_RETURN(false);
    }
    if(!recipient.isEmpty()&&!_recipient_re.match(recipient).hasMatch()) {
        LOG_DEBUG("rejecting "<<op<<" because \""<<recipient<<"\" is not matched by "<<_recipient_re.pattern());
        LOG_BOOL_RETURN(false);
    }
    if(!description.isEmpty()&&!_description_re.match(description).hasMatch()) {
        LOG_DEBUG("rejecting "<<op<<" because \""<<description<<"\" is not matched by "<<_description_re.pattern());
        LOG_BOOL_RETURN(false);
    }
    LOG_DEBUG("accepting " << op);
    LOG_BOOL_RETURN(true);
}
