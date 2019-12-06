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
#include "semver.h"
#include "utils/macro.h"

#include <QRegularExpression>

SemVer::~SemVer()
{

}

SemVer::SemVer(int major, int minor, int patch) :
    m_major(major),
    m_minor(minor),
    m_patch(patch)
{

}

SemVer::SemVer(const QString &version_str)
{
    QRegularExpressionMatch match;
    QRegularExpression semver_m("(?<major>\\d+)");
    QRegularExpression semver_mm("(?<major>\\d+)\\.(?<minor>\\d+)");
    QRegularExpression semver_mmp("(?<major>\\d+)\\.(?<minor>\\d+)\\.(?<patch>\\d+)");
    match=semver_mmp.match(version_str);
    if(match.hasMatch()) {
        m_major=match.captured("major").toInt();
        m_minor=match.captured("minor").toInt();
        m_patch=match.captured("patch").toInt();
        return;
    }
    match=semver_mm.match(version_str);
    if(match.hasMatch()) {
        m_major=match.captured("major").toInt();
        m_minor=match.captured("minor").toInt();
        m_patch=0;
        return;
    }
    match=semver_m.match(version_str);
    if(match.hasMatch()) {
        m_major=match.captured("major").toInt();
        m_minor=0;
        m_patch=0;
        return;
    }
    m_major=-1;
    m_minor=-1;
    m_patch=-1;
}

SemVer::SemVer(const SemVer &other) :
    m_major(other.m_major),
    m_minor(other.m_minor),
    m_patch(other.m_patch)
{

}

SemVer &SemVer::operator=(const SemVer &other)
{
    m_major=other.m_major;
    m_minor=other.m_minor;
    m_patch=other.m_patch;
    return *this;
}

QString SemVer::to_str(SemVer::Format fmt) const
{
    switch (fmt) {
    case SemVer::M_FORMAT: return QString("%0").arg(m_major);
    case SemVer::MM_FORMAT: return QString("%0.%1").arg(m_major).arg(m_minor);
    case SemVer::MMP_FORMAT: return QString("%0.%1.%2").arg(m_major).arg(m_minor).arg(m_patch);
    }
    return QString();
}


bool SemVer::operator!=(const SemVer &other) const
{
    return (m_major!=other.m_major||m_minor!=other.m_minor||m_patch!=other.m_patch);
}

bool SemVer::operator==(const SemVer &other) const
{
    return !operator!=(other);
}

bool SemVer::operator>(const SemVer &other) const
{
    return ((m_major>other.m_major)||
            (m_major==other.m_major&&(m_minor>other.m_minor||
                                    (m_minor==other.m_minor&&m_patch>other.m_patch))));
}

bool SemVer::operator<(const SemVer &other) const
{
    return ((m_major<other.m_major)||
            (m_major==other.m_major&&(m_minor<other.m_minor||
                                    (m_minor==other.m_minor&&m_patch<other.m_patch))));
}

bool SemVer::operator>=(const SemVer &other) const
{
    return !operator<(other);
}


bool SemVer::operator<=(const SemVer &other) const
{
    return !operator>(other);
}

QDebug operator<<(QDebug debug, const SemVer &semver)
{
    debug<<"SemVer("<<semver.major_num()<<","
                    <<semver.minor_num()<<","
                    <<semver.patch_num()<<")";
    return debug;
}
