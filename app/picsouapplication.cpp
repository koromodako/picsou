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
#include "utils/macro.h"

PicsouApplication::~PicsouApplication()
{
    LOG_IN_VOID();
    delete m_ui_svc;
    delete m_model_svc;
    LOG_VOID_RETURN();
}

PicsouApplication::PicsouApplication(QObject *parent) :
    QObject(parent)
{
    LOG_IN("parent="<<parent);
    m_ui_svc=new PicsouUIService(this);
    m_model_svc=new PicsouModelService(this);
    LOG_VOID_RETURN();
}

bool PicsouApplication::initialize()
{
    LOG_IN_VOID();
    if(!m_model_svc->initialize()) {
        LOG_CRITICAL("model controller initialization failed.");
        LOG_BOOL_RETURN(false);
    }
    if(!m_ui_svc->initialize()) {
        LOG_CRITICAL("ui controller initialization failed.");
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

void PicsouApplication::terminate()
{
    LOG_IN_VOID();
    m_ui_svc->terminate();
    m_model_svc->terminate();
    LOG_VOID_RETURN();
}
