#include "picsouui.h"

PicsouUI::~PicsouUI()
{
    /* do not delete _ui_svc here, this class does not have the ownership */
}

PicsouUI::PicsouUI(PicsouUIService *ui_svc) :
    _ui_svc(ui_svc)
{

}
