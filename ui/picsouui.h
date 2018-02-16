#ifndef PICSOUUI_H
#define PICSOUUI_H

class PicsouUIService;

class PicsouUI
{
public:
    virtual ~PicsouUI();
    explicit PicsouUI(PicsouUIService *ui_svc);

protected:
    inline PicsouUIService *ui_svc() { return _ui_svc; }

private:
    PicsouUIService *_ui_svc;
};

#endif // PICSOUUI_H
