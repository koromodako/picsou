#ifndef PICSOUAPPLICATION_H
#define PICSOUAPPLICATION_H

#include <QObject>

class PicsouUIService;
class PicsouModelService;
class PicsouCryptoService;
class PicsouConsoleService;

class PicsouApplication : public QObject
{
    Q_OBJECT
public:
    virtual ~PicsouApplication();
    explicit PicsouApplication(QObject *parent=nullptr);

    bool initialize();

    inline PicsouUIService *ui_svc() { return _ui_svc; }
    inline PicsouModelService *model_svc() { return _model_svc; }
    inline PicsouCryptoService *crypto_svc() { return _crypto_svc; }
    inline PicsouConsoleService *console_svc() { return _console_svc; }

public slots:
    void terminate();

private:
    PicsouUIService *_ui_svc;
    PicsouModelService *_model_svc;
    PicsouCryptoService *_crypto_svc;
    PicsouConsoleService *_console_svc;
};

#endif // PICSOUAPPLICATION_H
