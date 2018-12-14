#include "picsoucryptoservice.h"
#include "utils/macro.h"

PicsouCryptoService::~PicsouCryptoService()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

PicsouCryptoService::PicsouCryptoService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    _crypto_mod()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

bool PicsouCryptoService::initialize()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(_crypto_mod.initialize());
}

void PicsouCryptoService::terminate()
{
    LOG_IN_VOID();
    _crypto_mod.terminate();
    LOG_VOID_RETURN();
}
