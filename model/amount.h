#ifndef AMOUNT_H
#define AMOUNT_H

#include <QString>

class Amount
{
public:
    virtual ~Amount();
    Amount(int value);
    Amount(double value=0);

    inline double value() const { return _value; }

    QString to_str(bool add_currency=false) const;

    inline Amount operator+(const Amount &other) const { return Amount(_value+other._value); }
    inline Amount operator-(const Amount &other) const { return Amount(_value-other._value); }
    inline Amount operator/(const Amount &other) const { return Amount(_value/other._value); }
    inline Amount operator*(const Amount &other) const { return Amount(_value*other._value); }

    inline const Amount &operator+=(const Amount &other) { _value+=other._value; return *this; }
    inline const Amount &operator-=(const Amount &other) { _value-=other._value; return *this; }
    inline const Amount &operator/=(const Amount &other) { _value/=other._value; return *this; }
    inline const Amount &operator*=(const Amount &other) { _value*=other._value; return *this; }

    inline bool operator<(const Amount &other) const { return _value<other._value; }
    inline bool operator>(const Amount &other) const { return _value>other._value; }
    inline bool operator<=(const Amount &other) const { return _value<=other._value; }
    inline bool operator>=(const Amount &other) const { return _value>=other._value; }

    inline bool operator==(const Amount &other) const { return _value==other._value; }
    inline bool operator!=(const Amount &other) const { return _value!=other._value; }

private:
    double _value;
};

#endif // AMOUNT_H
