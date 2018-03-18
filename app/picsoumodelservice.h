#ifndef PICSOUMODELSERVICE_H
#define PICSOUMODELSERVICE_H

#include <QFile>
#include <QUuid>

#include "model/picsoudb.h"
#include "picsouabstractservice.h"

class PicsouModelService : public PicsouAbstractService
{
    Q_OBJECT
public:
    enum ImportExportFormat {
        CSV,
        XML,
        JSON
    };

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

    QList<OperationPtr> load_ops(ImportExportFormat fmt,
                                 QString filename);
    bool dump_ops(ImportExportFormat fmt,
                  QString filename,
                  QList<OperationPtr> ops);

    inline const PicsouDBPtr db() const { return _db; }
    inline bool is_db_modified() const { return _is_db_modified; }

    UserPtr find_user(QUuid id) const;
    AccountPtr find_account(QUuid id) const;

signals:
    void updated(const PicsouDBPtr db);

public slots:
    void notify_ui();

private:
    QList<OperationPtr> xml_load_ops(QFile &f);
    QList<OperationPtr> csv_load_ops(QFile &f);
    QList<OperationPtr> json_load_ops(QFile &f);

    bool xml_dump_ops(QFile &f, QList<OperationPtr> ops);
    bool csv_dump_ops(QFile &f, QList<OperationPtr> ops);
    bool json_dump_ops(QFile &f, QList<OperationPtr> ops);

private:
    PicsouDBPtr _db;
    QString _filename;
    bool _is_db_modified;

};

#endif // PICSOUMODELSERVICE_H
