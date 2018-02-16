#include "picsouconsoleservice.h"

PicsouConsoleService::~PicsouConsoleService()
{

}

PicsouConsoleService::PicsouConsoleService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{

}

bool PicsouConsoleService::initialize()
{
    return true;
}

void PicsouConsoleService::terminate()
{

}
