#include "picsoumodelservice.h"

PicsouModelService::~PicsouModelService()
{

}

PicsouModelService::PicsouModelService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{

}

bool PicsouModelService::initialize()
{
    return true;
}

void PicsouModelService::terminate()
{

}

