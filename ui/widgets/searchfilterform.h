#ifndef SEARCHFILTERFORM_H
#define SEARCHFILTERFORM_H

#include "ui/picsouui.h"
#include "model/searchquery.h"
#include <QWidget>

namespace Ui {
class SearchFilterForm;
}

class SearchFilterForm : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~SearchFilterForm();
    explicit SearchFilterForm(PicsouUIServicePtr ui_svc,
                              QWidget *parent = nullptr);

    SearchQuery query() const;

signals:
    void search_request();
    void search_update_failed(const QString &message);

public slots:
    void refresh_user_cb();
    void refresh_pms_list(const QString &account_name);
    void refresh_account_cb(const QString &username);
    void refresh_budgets_list(const QString &username);

private:
    Ui::SearchFilterForm *ui;
};

#endif // SEARCHFILTERFORM_H
