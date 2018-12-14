#include "picsouconsoleservice.h"
#include "utils/macro.h"

PicsouConsoleService::~PicsouConsoleService()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

PicsouConsoleService::PicsouConsoleService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

bool PicsouConsoleService::initialize()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(true);
}

void PicsouConsoleService::terminate()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}
