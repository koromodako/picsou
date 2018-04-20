#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QStyleFactory>

#include "app/picsouapplication.h"
#include "app/picsouuiservice.h"
#include "ui/mainwindow.h"

QCoreApplication* createApplication(int &argc, char *argv[])
{
  QCoreApplication *app=nullptr;

  for (int i=1; i < argc; ++i) {
      if (!qstrcmp(argv[i], "--no-gui")) {
          app=new QCoreApplication(argc, argv);
          goto end;
      }
  }

  QApplication::setStyle(QStyleFactory::create("Fusion"));

  app=new QApplication(argc, argv);
end:
  return app;
}

int main(int argc, char *argv[])
{
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
        exit(1);
    }

    if(qobject_cast<QApplication *>(app.data())) {
        /* start GUI version */
        papp.ui_svc()->show_mainwindow();
    } else {
       /* start non-GUI version */
        exit(1);
    }

    return app->exec();
}
