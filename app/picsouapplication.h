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
#ifndef PICSOUAPPLICATION_H
#define PICSOUAPPLICATION_H

#include <QObject>

class PicsouUIService;
class PicsouModelService;
class PicsouConsoleService;

class PicsouApplication : public QObject
{
    Q_OBJECT
public:
    virtual ~PicsouApplication();
    explicit PicsouApplication(QObject *parent=nullptr);

    bool initialize();

    inline PicsouUIService *ui_svc() { return _ui_svc; }
    inline PicsouModelService *model_svc() { return _model_svc; }
    inline PicsouConsoleService *console_svc() { return _console_svc; }

public slots:
    void terminate();

private:
    PicsouUIService *_ui_svc;
    PicsouModelService *_model_svc;
    PicsouConsoleService *_console_svc;
};

#endif // PICSOUAPPLICATION_H
