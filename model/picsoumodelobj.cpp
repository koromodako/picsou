#include "picsoumodelobj.h"

PicsouModelObj::PicsouModelObj(bool valid) :
    _valid(valid),
    _id(QUuid::createUuid())
{

}

PicsouModelObj::~PicsouModelObj()
{

}
