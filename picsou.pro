#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T19:46:54
#
#-------------------------------------------------

QT += core widgets

TARGET = picsou
TEMPLATE = app

CONFIG += console c++14

# Run prebuild script
system(./prebuild)
# Force qmake to run
qmakeforce.target = dummy
qmakeforce.commands = rm -f Makefile ##to force rerun of qmake
qmakeforce.depends = FORCE
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
    ui/mainwindow.cpp \
    model/scheduledoperation.cpp \
    model/picsoumodelobj.cpp \
    model/paymentmethod.cpp \
    model/operation.cpp \
    model/picsoudb.cpp \
    model/account.cpp \
    model/budget.cpp \
    model/user.cpp \
    utils/crypto/qmessageauthcode.cpp \
    utils/crypto/qcryptowrapper.cpp \
    utils/crypto/qsecurememory.cpp \
    utils/crypto/qcryptomodule.cpp \
    utils/crypto/qrandombytes.cpp \
    utils/crypto/qpassphrase.cpp \
    utils/crypto/qsecurehash.cpp \
    utils/crypto/qcipher.cpp \
    utils/crypto/qnonce.cpp \
    app/picsouapplication.cpp \
    app/picsoucryptoservice.cpp \
    app/picsoumodelservice.cpp \
    app/picsouuiservice.cpp \
    app/picsouconsoleservice.cpp \
    app/picsouabstractservice.cpp \
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
    model/operationcollection.cpp \
    ui/widgets/operationstatistics.cpp \
    model/amount.cpp

HEADERS += \
    picsou.h \
    picsou.h.dist \
    model/picsoudb.h \
    model/user.h \
    model/budget.h \
    model/account.h \
    model/paymentmethod.h \
    model/operation.h \
    model/scheduledoperation.h \
    model/picsoumodelobj.h \
    utils/crypto/qmessageauthcode.h \
    utils/crypto/qcryptowrapper.h \
    utils/crypto/qrandombytes.h \
    utils/crypto/qsecurememory.h \
    utils/crypto/qcryptomodule.h \
    utils/crypto/qpassphrase.h \
    utils/crypto/qsecurehash.h \
    utils/crypto/qcipher.h \
    utils/crypto/qnonce.h \
    utils/macro.h \
    app/picsouapplication.h \
    app/picsoucryptoservice.h \
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
    model/amount.h

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
    tests/data/input.csv

TRANSLATIONS += \
    translation/picsou_fr_FR.ts
