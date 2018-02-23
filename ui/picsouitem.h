#ifndef PICSOUITEM_H
#define PICSOUITEM_H

#include <QUuid>

class PicsouItem
{
public:
    virtual ~PicsouItem();

    inline QUuid mod_obj_id() const { return _id; }

protected:
    PicsouItem(QUuid id);

private:
    QUuid _id;
};

#endif // PICSOUITEM_H
