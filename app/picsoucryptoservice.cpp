#include "picsoucryptoservice.h"
#include "utils/macro.h"

PicsouCryptoService::~PicsouCryptoService()
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

PicsouCryptoService::PicsouCryptoService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    _crypto_mod()
{
    LOG_IN_VOID();
    LOG_OUT_VOID();
}

bool PicsouCryptoService::initialize()
{
    LOG_IN_VOID();
    bool success=_crypto_mod.initialize();
    LOG_OUT("success="<<bool2str(success));
    return success;
}

void PicsouCryptoService::terminate()
{
    LOG_IN_VOID();
    _crypto_mod.terminate();
    LOG_OUT_VOID();
}
