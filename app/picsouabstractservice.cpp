#include "picsouabstractservice.h"


PicsouAbstractService::~PicsouAbstractService()
{
    /* do not delete papp here, this class does not have the ownership */
}

PicsouAbstractService::PicsouAbstractService(PicsouApplication *papp) :
    _papp(papp)
{

}
