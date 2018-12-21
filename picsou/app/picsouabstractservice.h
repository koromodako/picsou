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
#ifndef PICSOUABSTRACTSERVICE_H
#define PICSOUABSTRACTSERVICE_H

#include <QObject>

class PicsouApplication;

class PicsouAbstractService : public QObject
{
    Q_OBJECT
public:
    virtual ~PicsouAbstractService();

    virtual bool initialize()=0;
    virtual void terminate()=0;

protected:
    explicit PicsouAbstractService(PicsouApplication *papp);

    inline PicsouApplication *papp() { return m_papp; }

private:
    PicsouApplication *m_papp;
};

#endif // PICSOUABSTRACTSERVICE_H