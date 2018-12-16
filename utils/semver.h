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
#ifndef SEMVER_H
#define SEMVER_H

#include <QString>

class SemVer
{
public:
    virtual ~SemVer();
    SemVer(int major=-1, int minor=0, int patch=0);
    SemVer(const QString &version_str);
    SemVer(const SemVer &other);

    enum Format {
        MMP_FORMAT,
        MM_FORMAT,
        M_FORMAT
    };

    inline int major_num() const { return _major; }
    inline int minor_num() const { return _minor; }
    inline int patch_num() const { return _patch; }

    inline bool is_valid() const { return !(_major<0||_minor<0||_patch<0); }

    QString to_str(Format fmt=MMP_FORMAT) const;

    bool operator!=(const SemVer &other) const;
    bool operator==(const SemVer &other) const;
    bool operator<(const SemVer &other) const;
    bool operator>(const SemVer &other) const;
    bool operator<=(const SemVer &other) const;
    bool operator>=(const SemVer &other) const;


private:
    int _major;
    int _minor;
    int _patch;
};

QDebug operator<<(QDebug debug, const SemVer &semver);


#endif // SEMVER_H
