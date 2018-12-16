#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T19:46:54
#
#-------------------------------------------------

QT += core widgets concurrent

TARGET=picsou
TEMPLATE=app

CONFIG += static console c++14

DEFINES += COLORIZE

# Run prebuild script
system(./prebuild)
# Force qmake to run
qmakeforce.target=dummy
qmakeforce.commands=rm -f Makefile ##to force rerun of qmake
qmakeforce.depends=FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += GCRYPT_NO_DEPRECATED

LIBS += -lgcrypt

QMAKE_CXXFLAGS += -Wall \
                  -Werror \
                  -pedantic-errors

SOURCES += \
    main.cpp \
    utils/semver.cpp \
    model/object/scheduledoperation.cpp \
    model/object/paymentmethod.cpp \
    model/object/operation.cpp \
    model/object/schedule.cpp \
    model/object/account.cpp \
    model/object/budget.cpp \
    model/object/amount.cpp \
    model/object/user.cpp \
    model/searchquery.cpp \
    model/operationcollection.cpp \
    model/picsoumodelobj.cpp \
    model/picsoudb.cpp \
    model/converter/converter.cpp \
    app/picsouapplication.cpp \
    app/picsoumodelservice.cpp \
    app/picsouuiservice.cpp \
    app/picsouconsoleservice.cpp \
    app/picsouabstractservice.cpp \
    ui/mainwindow.cpp \
    ui/dialogs/paymentmethodeditor.cpp \
    ui/dialogs/scheduledoperationeditor.cpp \
    ui/dialogs/operationeditor.cpp \
    ui/dialogs/picsoudbeditor.cpp \
    ui/dialogs/accounteditor.cpp \
    ui/dialogs/budgeteditor.cpp \
    ui/dialogs/usereditor.cpp \
    ui/dialogs/importdialog.cpp \
    ui/dialogs/aboutpicsou.cpp \
    ui/viewers/operationviewer.cpp \
    ui/viewers/picsoudbviewer.cpp \
    ui/viewers/accountviewer.cpp \
    ui/viewers/userviewer.cpp \
    ui/items/picsoulistitem.cpp \
    ui/items/picsoutreeitem.cpp \
    ui/picsouuiviewer.cpp \
    ui/picsouui.cpp \
    ui/picsouitem.cpp \
    ui/items/picsoutableitem.cpp \
    ui/widgets/picsoutablewidget.cpp \
    ui/widgets/operationstatistics.cpp

HEADERS += \
    picsou.h \
    picsou.h.dist \
    model/object/scheduledoperation.h \
    model/object/paymentmethod.h \
    model/object/operation.h \
    model/object/account.h \
    model/object/budget.h \
    model/object/amount.h \
    model/object/user.h \
    model/searchquery.h \
    model/picsoudb.h \
    model/picsoumodelobj.h \
    utils/macro.h \
    utils/semver.h \
    app/picsouapplication.h \
    app/picsoumodelservice.h \
    app/picsouuiservice.h \
    app/picsouconsoleservice.h \
    app/picsouabstractservice.h \
    ui/dialogs/scheduledoperationeditor.h \
    ui/dialogs/paymentmethodeditor.h \
    ui/dialogs/operationeditor.h \
    ui/dialogs/picsoudbeditor.h \
    ui/dialogs/accounteditor.h \
    ui/dialogs/budgeteditor.h \
    ui/dialogs/usereditor.h \
    ui/dialogs/importdialog.h \
    ui/dialogs/aboutpicsou.h \
    ui/viewers/operationviewer.h \
    ui/viewers/picsoudbviewer.h \
    ui/viewers/accountviewer.h \
    ui/viewers/userviewer.h \
    ui/items/picsoulistitem.h \
    ui/items/picsoutreeitem.h \
    ui/picsouuiviewer.h \
    ui/mainwindow.h \
    ui/picsouui.h \
    ui/picsouitem.h \
    ui/items/picsoutableitem.h \
    ui/widgets/picsoutablewidget.h \
    model/operationcollection.h \
    ui/widgets/operationstatistics.h \
    model/converter/converter.h \
    model/object/schedule.h

FORMS += \
    ui/mainwindow.ui \
    ui/dialogs/scheduledoperationeditor.ui \
    ui/dialogs/paymentmethodeditor.ui \
    ui/dialogs/operationeditor.ui \
    ui/dialogs/picsoudbeditor.ui \
    ui/dialogs/accounteditor.ui \
    ui/dialogs/budgeteditor.ui \
    ui/dialogs/usereditor.ui \
    ui/dialogs/importdialog.ui \
    ui/dialogs/aboutpicsou.ui \
    ui/viewers/operationviewer.ui \
    ui/viewers/picsoudbviewer.ui \
    ui/viewers/accountviewer.ui \
    ui/viewers/userviewer.ui \
    ui/widgets/operationstatistics.ui

RESOURCES += \
    picsou.qrc

DISTFILES += \
    TODO.md \
    README.md \
    tests/data/input.json \
    tests/data/input.xml \
    tests/data/input.csv \
    prebuild

TRANSLATIONS += \
    translation/picsou_fr_FR.ts
