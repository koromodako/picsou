#include "app/picsouapplication.h"
#include "app/picsouuiservice.h"
#include "ui/mainwindow.h"
#include "utils/macro.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QStyleFactory>

QCoreApplication* createApplication(int &argc, char *argv[])
{
    LOG_IN("argc="<<argc<<",argv="<<argv);
    QCoreApplication *app=nullptr;
    for(int i=1; i < argc; ++i) {
        if(!qstrcmp(argv[i], "--no-gui")) {
            app=new QCoreApplication(argc, argv);
            LOG_INFO("-> app[console]="<<app);
            return app;
        }
    }
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    app=new QApplication(argc, argv);
    LOG_INFO("-> app[gui]="<<app);
    return app;
}

int main(int argc, char *argv[])
{
    LOG_IN("argc="<<argc<<",argv="<<argv);
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app->installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("picsou_" + QLocale::system().name(), ":/translation");
    app->installTranslator(&appTranslator);

    PicsouApplication papp(app.data());

    QObject::connect(app.data(), &QCoreApplication::aboutToQuit,
                     &papp, &PicsouApplication::terminate);

    if(!papp.initialize()) {
        LOG_CRITICAL("failed to initialize application.");
        return 1;
    }
    if(qobject_cast<QApplication *>(app.data())) {
        /* start GUI version */
        papp.ui_svc()->show_mainwindow();
    } else {
       /* start non-GUI version */
        LOG_INFO("test");
        LOG_CRITICAL("no CLI implemented yet...");
        return 1;
    }
    int rcode=app->exec();
    LOG_INFO("-> rcode="<<rcode);
    return rcode;
}
