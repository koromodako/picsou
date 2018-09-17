#include "amount.h"
#include "utils/macro.h"

#include <QObject>

Amount::~Amount()
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

Amount::Amount(int value) :
    _value(value)
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

Amount::Amount(double value) :
    _value(value)
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

Amount::Amount(const Amount &other) :
    _value(other._value)
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

QString Amount::to_str(bool add_currency) const
{
    LOG_IN("add_currency="<<bool2str(add_currency));
    QString str;
    if(add_currency) {
        str=QObject::tr("$%0").arg(_value, 0, 'f', 2);
    } else {
        str=QString("%0").arg(_value, 0, 'f', 2);
    }
    LOG_OUT("str="<<str);
    return str;
}

QDebug operator<<(QDebug debug, const Amount &amount)
{
    debug<<"Amount("<<amount.value()<<")";
    return debug;
}
