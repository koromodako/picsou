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

    OperationCollection load_ops(ImportExportFormat fmt,
                                 QString filename);
    bool dump_ops(ImportExportFormat fmt,
                  QString filename,
                  OperationCollection ops);

    inline const PicsouDBPtr db() const { return m_db; }
    inline bool is_db_modified() const { return m_is_db_modified; }

    UserPtr find_user(QUuid id) const;
    AccountPtr find_account(QUuid id) const;

signals:
    void updated(const PicsouDBPtr db);

public slots:
    void notify_ui();

private:
    OperationCollection xml_load_ops(QFile &f);
    OperationCollection csv_load_ops(QFile &f);
    OperationCollection json_load_ops(QFile &f);

    bool xml_dump_ops(QFile &f, OperationCollection ops);
    bool csv_dump_ops(QFile &f, OperationCollection ops);
    bool json_dump_ops(QFile &f, OperationCollection ops);

private:
    PicsouDBPtr m_db;
    QString m_filename;
    bool m_is_db_modified;

};

#endif // PICSOUMODELSERVICE_H
