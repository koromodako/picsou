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
    LOG_OUT_VOID();
}

PicsouApplication::PicsouApplication(QObject *parent) :
    QObject(parent)
{
    LOG_IN("parent="<<parent);
    _ui_svc=new PicsouUIService(this);
    _model_svc=new PicsouModelService(this);
    _crypto_svc=new PicsouCryptoService(this);
    _console_svc=new PicsouConsoleService(this);
    LOG_OUT_VOID();
}

bool PicsouApplication::initialize()
{
    LOG_IN_VOID();
    bool success=false;
    if(!_crypto_svc->initialize()) {
        goto end;
    }
    if(!_model_svc->initialize()) {
        goto end;
    }
    if(!_console_svc->initialize()) {
        goto end;
    }
    if(!_ui_svc->initialize()) {
        goto end;
    }
    success=true;
end:
    LOG_OUT("success="<<success);
    return success;
}

void PicsouApplication::terminate()
{
    LOG_IN_VOID();
    _ui_svc->terminate();
    _console_svc->terminate();
    _model_svc->terminate();
    _crypto_svc->terminate();
    LOG_OUT_VOID();
}
