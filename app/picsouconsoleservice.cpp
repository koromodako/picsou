#include "picsouconsoleservice.h"
#include "utils/macro.h"

PicsouConsoleService::~PicsouConsoleService()
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

PicsouConsoleService::PicsouConsoleService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

bool PicsouConsoleService::initialize()
{
    LOG_IN_VOID();
    int success=true;
    LOG_OUT("success="<<success);
    return success;
}

void PicsouConsoleService::terminate()
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}
