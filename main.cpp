#include <QApplication>

#include "app/picsouapplication.h"
#include "ui/mainwindow.h"

QCoreApplication* createApplication(int &argc, char *argv[])
{
  QCoreApplication *app=nullptr;

  for (int i = 1; i < argc; ++i) {
      if (!qstrcmp(argv[i], "--no-gui")) {
          app=new QCoreApplication(argc, argv);
          goto end;
      }
  }

  app=new QApplication(argc, argv);
end:
  return app;
}

int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

    PicsouApplication papp(app.data());

    QObject::connect(app.data(), &QCoreApplication::aboutToQuit,
                     &papp, &PicsouApplication::terminate);

    if(!papp.initialize()) {
        exit(1);
    }

    if(qobject_cast<QApplication *>(app.data())) {
        /* start GUI version */
        MainWindow w;
        w.show();
    } else {
       /* start non-GUI version */
        exit(1);
    }

    return app->exec();
}
