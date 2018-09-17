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
