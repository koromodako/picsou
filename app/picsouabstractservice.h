#ifndef PICSOUABSTRACTSERVICE_H
#define PICSOUABSTRACTSERVICE_H

#include <QObject>

class PicsouApplication;

class PicsouAbstractService : public QObject
{
    Q_OBJECT
public:
    virtual ~PicsouAbstractService();

    virtual bool initialize() = 0;
    virtual void terminate() = 0;

protected:
    explicit PicsouAbstractService(PicsouApplication *papp);

    inline PicsouApplication *papp() { return _papp; }

private:
    PicsouApplication *_papp;
};

#endif // PICSOUABSTRACTSERVICE_H
