#include "semver.h"
#include "utils/macro.h"

#include <QRegularExpression>

SemVer::~SemVer()
{

}

SemVer::SemVer(int major, int minor, int patch) :
    _major(major),
    _minor(minor),
    _patch(patch)
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
        _major=match.captured("major").toInt();
        _minor=match.captured("minor").toInt();
        _patch=match.captured("patch").toInt();
        return;
    }
    match=semver_mm.match(version_str);
    if(match.hasMatch()) {
        _major=match.captured("major").toInt();
        _minor=match.captured("minor").toInt();
        _patch=0;
        return;
    }
    match=semver_m.match(version_str);
    if(match.hasMatch()) {
        _major=match.captured("major").toInt();
        _minor=0;
        _patch=0;
        return;
    }
    _major=-1;
    _minor=-1;
    _patch=-1;
}

SemVer::SemVer(const SemVer &other) :
    _major(other._major),
    _minor(other._minor),
    _patch(other._patch)
{

}

QString SemVer::to_str(SemVer::Format fmt) const
{
    switch (fmt) {
    case SemVer::M_FORMAT: return QString("%0").arg(_major);
    case SemVer::MM_FORMAT: return QString("%0.%1").arg(_major).arg(_minor);
    case SemVer::MMP_FORMAT: return QString("%0.%1.%2").arg(_major).arg(_minor).arg(_patch);
    }
}


bool SemVer::operator!=(const SemVer &other) const
{
    return (_major!=other._major||_minor!=other._minor||_patch!=other._patch);
}

bool SemVer::operator==(const SemVer &other) const
{
    return !operator!=(other);
}

bool SemVer::operator>(const SemVer &other) const
{
    return ((_major>other._major)||
            (_major==other._major&&(_minor>other._minor||
                                    (_minor==other._minor&&_patch>other._patch))));
}

bool SemVer::operator<(const SemVer &other) const
{
    return ((_major<other._major)||
            (_major==other._major&&(_minor<other._minor||
                                    (_minor==other._minor&&_patch<other._patch))));
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
