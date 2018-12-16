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
#include "picsouapplication.h"
#include "picsouuiservice.h"
#include "picsoumodelservice.h"
#include "picsouconsoleservice.h"
#include "utils/macro.h"

PicsouApplication::~PicsouApplication()
{
    LOG_IN_VOID();
    delete _console_svc;
    delete _ui_svc;
    delete _model_svc;
    LOG_VOID_RETURN();
}

PicsouApplication::PicsouApplication(QObject *parent) :
    QObject(parent)
{
    LOG_IN("parent="<<parent);
    _ui_svc=new PicsouUIService(this);
    _model_svc=new PicsouModelService(this);
    _console_svc=new PicsouConsoleService(this);
    LOG_VOID_RETURN();
}

bool PicsouApplication::initialize()
{
    LOG_IN_VOID();
    if(!_model_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    if(!_console_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    if(!_ui_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

void PicsouApplication::terminate()
{
    LOG_IN_VOID();
    _ui_svc->terminate();
    _console_svc->terminate();
    _model_svc->terminate();
    LOG_VOID_RETURN();
}
