#include "picsoucryptoservice.h"

PicsouCryptoService::~PicsouCryptoService()
{

}

PicsouCryptoService::PicsouCryptoService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    _crypto_mod()
{

}

bool PicsouCryptoService::initialize()
{
    return _crypto_mod.initialize();
}

void PicsouCryptoService::terminate()
{
    _crypto_mod.terminate();
}
