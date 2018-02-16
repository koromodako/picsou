#ifndef PICSOUABSTRACTSERVICE_H
#define PICSOUABSTRACTSERVICE_H

#include <QObject>

class PicsouApplication;

class PicsouAbstractService : public QObject
{
    Q_OBJECT
public:
    virtual ~PicsouAbstractService();
    explicit PicsouAbstractService(PicsouApplication *papp);

    virtual bool initialize() = 0;
    virtual void terminate() = 0;

protected:
    inline PicsouApplication *papp() { return _papp; }

private:
    PicsouApplication *_papp;
};

#endif // PICSOUABSTRACTSERVICE_H
