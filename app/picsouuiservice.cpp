#include "picsouuiservice.h"

#include "ui/mainwindow.h"

PicsouUIService::~PicsouUIService()
{
    delete _mw;
}

PicsouUIService::PicsouUIService(PicsouApplication *papp) :
    PicsouAbstractService(papp)
{
    _mw = new MainWindow(this);
}

bool PicsouUIService::initialize()
{
    return true;
}

void PicsouUIService::terminate()
{

}

void PicsouUIService::show_mainwindow()
{
    _mw->show();
}
