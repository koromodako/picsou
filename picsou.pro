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
    model/picsoudb.cpp \
    model/user.cpp \
    model/budget.cpp \
    model/account.cpp \
    model/paymentmethod.cpp \
    model/operation.cpp \
    model/scheduledoperation.cpp \
    model/picsoumodelobj.cpp \
    utils/crypto/qmessageauthcode.cpp \
    utils/crypto/qrandombytes.cpp \
    utils/crypto/qpassphrase.cpp \
    utils/crypto/qnonce.cpp \
    utils/crypto/qsecurememory.cpp \
    utils/crypto/qsecurehash.cpp \
    utils/crypto/qcipher.cpp \
    utils/crypto/qcryptomodule.cpp \
    app/picsouapplication.cpp \
    utils/crypto/qcryptowrapper.cpp \
    app/picsoucryptoservice.cpp \
    app/picsoumodelservice.cpp \
    app/picsouuiservice.cpp \
    app/picsouconsoleservice.cpp \
    app/picsouabstractservice.cpp \
    ui/budgeteditor.cpp \
    ui/accounteditor.cpp \
    ui/userviewer.cpp \
    ui/accountviewer.cpp \
    ui/paymentmethodeditor.cpp \
    ui/scheduledoperationeditor.cpp \
    ui/operationeditor.cpp \
    ui/usereditor.cpp \
    ui/picsoudbviewer.cpp \
    ui/picsouui.cpp \
    ui/aboutpicsou.cpp \
    ui/picsoudbeditor.cpp

HEADERS += \
    ui/mainwindow.h \
    model/picsoudb.h \
    model/user.h \
    model/budget.h \
    model/account.h \
    model/paymentmethod.h \
    model/operation.h \
    model/scheduledoperation.h \
    model/picsoumodelobj.h \
    utils/crypto/qmessageauthcode.h \
    utils/crypto/qrandombytes.h \
    utils/crypto/qpassphrase.h \
    utils/crypto/qnonce.h \
    utils/crypto/qsecurememory.h \
    utils/crypto/qsecurehash.h \
    utils/macro.h \
    utils/crypto/qcipher.h \
    utils/crypto/qcryptomodule.h \
    app/picsouapplication.h \
    utils/crypto/qcryptowrapper.h \
    app/picsoucryptoservice.h \
    app/picsoumodelservice.h \
    app/picsouuiservice.h \
    app/picsouconsoleservice.h \
    app/picsouabstractservice.h \
    ui/budgeteditor.h \
    ui/accounteditor.h \
    ui/userviewer.h \
    ui/accountviewer.h \
    ui/paymentmethodeditor.h \
    ui/scheduledoperationeditor.h \
    ui/operationeditor.h \
    ui/usereditor.h \
    ui/picsoudbviewer.h \
    ui/picsouui.h \
    ui/aboutpicsou.h \
    picsou.h \
    ui/picsoudbeditor.h

FORMS += \
        ui/mainwindow.ui \
    ui/budgeteditor.ui \
    ui/accounteditor.ui \
    ui/userviewer.ui \
    ui/accountviewer.ui \
    ui/paymentmethodeditor.ui \
    ui/scheduledoperationeditor.ui \
    ui/operationeditor.ui \
    ui/usereditor.ui \
    ui/picsoudbviewer.ui \
    ui/aboutpicsou.ui \
    ui/picsoudbeditor.ui
