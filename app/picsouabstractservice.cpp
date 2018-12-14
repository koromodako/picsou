#include "picsouabstractservice.h"
#include "utils/macro.h"

PicsouAbstractService::~PicsouAbstractService()
{
    LOG_IN_VOID();
    /* do not delete papp here, this class does not have the ownership */
    LOG_VOID_RETURN();
}

PicsouAbstractService::PicsouAbstractService(PicsouApplication *papp) :
    _papp(papp)
{
    LOG_IN("papp="<<papp);
    LOG_VOID_RETURN();
}
