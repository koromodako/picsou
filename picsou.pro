#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T19:46:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = picsou
TEMPLATE = app

CONFIG += c++11

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

QMAKE_CXXFLAGS += -W \
                  -Wall \
                  -Werror

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
    ui/editors/paymentmethodeditor.cpp \
    ui/editors/scheduledoperationeditor.cpp \
    ui/editors/operationeditor.cpp \
    ui/editors/picsoudbeditor.cpp \
    ui/editors/accounteditor.cpp \
    ui/editors/budgeteditor.cpp \
    ui/editors/usereditor.cpp \
    ui/viewers/operationviewer.cpp \
    ui/viewers/picsoudbviewer.cpp \
    ui/viewers/accountviewer.cpp \
    ui/viewers/userviewer.cpp \
    ui/items/picsoulistitem.cpp \
    ui/items/picsoutreeitem.cpp \
    ui/picsouuiviewer.cpp \
    ui/aboutpicsou.cpp \
    ui/picsouui.cpp \
    ui/picsouitem.cpp \
    ui/items/picsoutableitem.cpp

HEADERS += \
    picsou.h \
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
    ui/editors/scheduledoperationeditor.h \
    ui/editors/paymentmethodeditor.h \
    ui/editors/operationeditor.h \
    ui/editors/picsoudbeditor.h \
    ui/editors/accounteditor.h \
    ui/editors/budgeteditor.h \
    ui/editors/usereditor.h \
    ui/viewers/operationviewer.h \
    ui/viewers/picsoudbviewer.h \
    ui/viewers/accountviewer.h \
    ui/viewers/userviewer.h \
    ui/items/picsoulistitem.h \
    ui/items/picsoutreeitem.h \
    ui/picsouuiviewer.h \
    ui/aboutpicsou.h \
    ui/mainwindow.h \
    ui/picsouui.h \
    ui/picsouitem.h \
    ui/items/picsoutableitem.h

FORMS += \
    ui/mainwindow.ui \
    ui/editors/scheduledoperationeditor.ui \
    ui/editors/paymentmethodeditor.ui \
    ui/editors/operationeditor.ui \
    ui/editors/picsoudbeditor.ui \
    ui/editors/accounteditor.ui \
    ui/editors/budgeteditor.ui \
    ui/editors/usereditor.ui \
    ui/viewers/operationviewer.ui \
    ui/viewers/picsoudbviewer.ui \
    ui/viewers/accountviewer.ui \
    ui/viewers/userviewer.ui \
    ui/aboutpicsou.ui

RESOURCES += \
    picsou.qrc
