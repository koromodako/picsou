#include "amount.h"

#include <QObject>

Amount::~Amount()
{

}

Amount::Amount(int value) :
    _value(value)
{

}

Amount::Amount(double value) :
    _value(value)
{

}

QString Amount::to_str(bool add_currency) const
{
    QString str;
    if(add_currency) {
        str=QObject::tr("$%0").arg(_value, 0, 'f', 2);
    } else {
        str=QString("%0").arg(_value, 0, 'f', 2);
    }
    return str;
}
