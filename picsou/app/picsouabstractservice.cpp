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
#include "picsouabstractservice.h"
#include "utils/macro.h"

PicsouAbstractService::~PicsouAbstractService()
{
    LOG_IN_VOID()
    /* do not delete papp here, this class does not have the ownership */
    LOG_VOID_RETURN()
}

PicsouAbstractService::PicsouAbstractService(PicsouApplication *papp) :
    m_papp(papp)
{
    LOG_IN("papp="<<papp)
    LOG_VOID_RETURN()
}
