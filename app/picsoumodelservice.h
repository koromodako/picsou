#ifndef PICSOUMODELSERVICE_H
#define PICSOUMODELSERVICE_H

#include <QUuid>

#include "model/picsoudb.h"
#include "picsouabstractservice.h"

class PicsouModelService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouModelService();
    explicit PicsouModelService(PicsouApplication *papp);

    bool initialize();
    void terminate();

    bool new_db(QString filename,
                QString name,
                QString description);
    bool open_db(QString filename);
    bool save_db();
    bool save_db_as(QString filename);
    bool close_db();
    bool is_db_opened();

    inline const PicsouDBPtr db() const { return _db; }
    inline bool is_db_modified() const { return _is_db_modified; }

    UserPtr find_user(QUuid id) const;
    AccountPtr find_account(QUuid id) const;

signals:
    void updated(const PicsouDBPtr db);

public slots:
    void notify_ui();

private:
    PicsouDBPtr _db;
    QString _filename;
    bool _is_db_modified;

};

#endif // PICSOUMODELSERVICE_H
