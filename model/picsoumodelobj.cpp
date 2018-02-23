#include "picsoumodelobj.h"

PicsouModelObj::~PicsouModelObj()
{

}

PicsouModelObj::PicsouModelObj(bool valid, PicsouModelObj *parent) :
    _id(QUuid::createUuid()),
    _valid(valid),
    _parent(parent)
{
    if(parent!=nullptr) {
        connect(this, &PicsouModelObj::modified, parent, &PicsouModelObj::modified);
    }
}

