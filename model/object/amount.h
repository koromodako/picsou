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
#ifndef AMOUNT_H
#define AMOUNT_H

#include <QString>

class Amount
{
public:
    Amount(int value);
    Amount(double value=0);
    Amount(const Amount &other);

    inline double value() const { return _value; }

    QString to_str(bool add_currency=false) const;

    inline Amount operator-() const { return Amount(-_value); }

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

    inline bool operator==(const Amount &other) const { return qFuzzyCompare(_value, other._value); }
    inline bool operator!=(const Amount &other) const { return !qFuzzyCompare(_value, other._value); }

private:
    double _value;
};

QDebug operator<<(QDebug debug, const Amount &amount);

#endif // AMOUNT_H
