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
#include "searchquery.h"
#include "utils/macro.h"

SearchQuery::SearchQuery(const QString &username,
                         const QString &account_name,
                         const QDate &from,
                         const QDate &to,
                         const Amount &min,
                         const Amount &max,
                         const QString &description_filter,
                         const QString &recipient_filter,
                         const QStringList &budgets,
                         const QStringList &pms) :
    m_username(username),
    m_account_name(account_name),
    m_from(from),
    m_to(to),
    m_min(min),
    m_max(max),
    m_budgets(budgets),
    m_pms(pms)
{
    LOG_IN("username="<<username
           <<",account_name="<<account_name
           <<",from="<<from
           <<",to="<<to
           <<",min="<<min
           <<",max="<<max
           <<",description_filter="<<description_filter
           <<",recipient_filter="<<recipient_filter
           <<",budgets="<<budgets
           <<",pms="<<pms);
    QString description_re=description_filter.isEmpty()?"*":description_filter;
    QString recipient_re=recipient_filter.isEmpty()?"*":recipient_filter;
    m_budgets.append("");
    m_description_re=QRegularExpression(description_re.replace("*", "\\w*"),
                                        QRegularExpression::CaseInsensitiveOption);
    m_recipient_re=QRegularExpression(recipient_re.replace("*", "\\w*"),
                                      QRegularExpression::CaseInsensitiveOption);
    m_description_re.optimize();
    m_recipient_re.optimize();
    LOG_VOID_RETURN();
}

bool SearchQuery::accepts(const OperationShPtr &op) const
{
    LOG_IN("op="<<op);
    QDate date=op->date();
    Amount amount=qAbs(op->amount());
    QString recipient=op->recipient();
    QString description=op->description();
    if(date<m_from||date>m_to) {
        LOG_DEBUG("rejecting "<<op<<" because "<<date<<"<"<<m_from<<"||"<<date<<">"<<m_to);
        LOG_BOOL_RETURN(false);
    }
    if(amount<m_min||amount>m_max) {
        LOG_DEBUG("rejecting "<<op<<" because "<<amount.value()<<"<"<<m_min.value()<<"||"<<amount.value()<<">"<<m_max.value());
        LOG_BOOL_RETURN(false);
    }
    if(!m_budgets.contains(op->budget())) {
        LOG_DEBUG("rejecting "<<op<<" because "<<op->budget()<<" not in "<<m_budgets);
        LOG_BOOL_RETURN(false);
    }
    if(!m_pms.contains(op->payment_method())) {
        LOG_DEBUG("rejecting "<<op<<" because "<<op->payment_method()<<" not in "<<m_pms);
        LOG_BOOL_RETURN(false);
    }
    if(!recipient.isEmpty()&&!m_recipient_re.match(recipient).hasMatch()) {
        LOG_DEBUG("rejecting "<<op<<" because \""<<recipient<<"\" is not matched by "<<m_recipient_re.pattern());
        LOG_BOOL_RETURN(false);
    }
    if(!description.isEmpty()&&!m_description_re.match(description).hasMatch()) {
        LOG_DEBUG("rejecting "<<op<<" because \""<<description<<"\" is not matched by "<<m_description_re.pattern());
        LOG_BOOL_RETURN(false);
    }
    LOG_DEBUG("accepting "<<op);
    LOG_BOOL_RETURN(true);
}
