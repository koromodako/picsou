#include "picsouapplication.h"
#include "picsouuiservice.h"
#include "picsoumodelservice.h"
#include "picsoucryptoservice.h"
#include "picsouconsoleservice.h"
#include "utils/macro.h"

PicsouApplication::~PicsouApplication()
{
    LOG_IN_VOID();
    delete _console_svc;
    delete _ui_svc;
    delete _model_svc;
    delete _crypto_svc;
    LOG_VOID_RETURN();
}

PicsouApplication::PicsouApplication(QObject *parent) :
    QObject(parent)
{
    LOG_IN("parent="<<parent);
    _ui_svc=new PicsouUIService(this);
    _model_svc=new PicsouModelService(this);
    _crypto_svc=new PicsouCryptoService(this);
    _console_svc=new PicsouConsoleService(this);
    LOG_VOID_RETURN();
}

bool PicsouApplication::initialize()
{
    LOG_IN_VOID();
    if(!_crypto_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    if(!_model_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    if(!_console_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    if(!_ui_svc->initialize()) {
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

void PicsouApplication::terminate()
{
    LOG_IN_VOID();
    _ui_svc->terminate();
    _console_svc->terminate();
    _model_svc->terminate();
    _crypto_svc->terminate();
    LOG_VOID_RETURN();
}
