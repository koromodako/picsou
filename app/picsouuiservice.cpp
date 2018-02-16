#include "picsouuiservice.h"

PicsouUIService::~PicsouUIService()
{

}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{

}

bool PicsouUIService::initialize()
{
    return true;
}

void PicsouUIService::terminate()
{

}
