/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "app/picsouapplication.h"
#include "app/picsouuiservice.h"
#include "ui/mainwindow.h"
#include "utils/macro.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    LOG_IN("argc="<<argc<<",argv="<<argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
    /* install Qt library translator */
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app->installTranslator(&qtTranslator);
    /* install Picsou app translator */
    QTranslator appTranslator;
    appTranslator.load("picsou_" + QLocale::system().name(), ":/translation");
    app->installTranslator(&appTranslator);
    /* Construct Picsou application */
    PicsouApplication papp(app.data());
    /* Connect termination signal */
    QObject::connect(app.data(), &QCoreApplication::aboutToQuit, &papp, &PicsouApplication::terminate);
    /* Initialize Picsou application */
    if(!papp.initialize()) {
        LOG_CRITICAL("failed to initialize application.");
        return 1;
    }
    papp.ui_svc()->show_mainwindow();
    int rcode=app->exec();
    LOG_DEBUG("-> rcode="<<rcode);
    return rcode;
}
