#include "picsouapplication.h"

#include "picsouuiservice.h"
#include "picsoumodelservice.h"
#include "picsoucryptoservice.h"
#include "picsouconsoleservice.h"

PicsouApplication::~PicsouApplication()
{
    delete _console_svc;
    delete _ui_svc;
    delete _model_svc;
    delete _crypto_svc;
}

PicsouApplication::PicsouApplication(QObject *parent) :
    QObject(parent)
{
    _ui_svc=new PicsouUIService(this);
    _model_svc=new PicsouModelService(this);
    _crypto_svc=new PicsouCryptoService(this);
    _console_svc=new PicsouConsoleService(this);
}

bool PicsouApplication::initialize()
{
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
    return success;
}

void PicsouApplication::terminate()
{
    _ui_svc->terminate();
    _console_svc->terminate();
    _model_svc->terminate();
    _crypto_svc->terminate();
}
