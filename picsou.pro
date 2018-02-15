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
    utils/crypto/qcrypto.cpp \
    utils/crypto/qmessageauthcode.cpp \
    utils/crypto/qrandombytes.cpp \
    utils/crypto/qpassphrase.cpp \
    utils/crypto/qnonce.cpp \
    utils/crypto/qsecurememory.cpp \
    utils/crypto/qsecurehash.cpp \
    utils/crypto/qcipher.cpp

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
    utils/crypto/qcrypto.h \
    utils/crypto/qmessageauthcode.h \
    utils/crypto/qrandombytes.h \
    utils/crypto/qpassphrase.h \
    utils/crypto/qnonce.h \
    utils/crypto/qsecurememory.h \
    utils/crypto/qsecurehash.h \
    utils/macro.h \
    utils/crypto/qcipher.h

FORMS += \
        ui/mainwindow.ui
