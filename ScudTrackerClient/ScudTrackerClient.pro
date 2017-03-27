#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T09:26:21
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScudTrackerClient
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../Common/log.cpp \
    ../../Common/sv_pgdb.cpp \
    ../../Common/sv_tcpserverclient.cpp \
    ../../Common/sv_userlogon.cpp \
    ../../Common/sv_secondmeter.cpp \
    ../../Common/sv_settings.cpp \
    sv_trackersettings.cpp \
    ../sv_select_device_dialog.cpp \
    ../ScudAdmin/sv_device.cpp \
    ../ScudAdmin/sv_device_config.cpp \
    ../ScudAdmin/sv_select_model_dialog.cpp \
    ../ScudAdmin/sv_dev_model.cpp \
    ../ScudAdmin/sv_dev_brand.cpp \
    ../ScudAdmin/sv_dev_class.cpp \
    ../ScudAdmin/sv_select_brand_dialog.cpp \
    ../ScudAdmin/sv_select_class_dialog.cpp

HEADERS  += mainwindow.h \
    ../../Common/log.h \
    ../../Common/sql_select_defs.h \
    ../../Common/sv_pgdb.h \
    ../../Common/sv_tcpserverclient.h \
    ../../Common/sv_userlogon.h \
    ../../Common/sv_secondmeter.h \
    ../../Common/sv_settings.h \
    sv_trackersettings.h \
    ../sv_select_device_dialog.h \
    ../ScudAdmin/sv_device.h \
    ../ScudAdmin/sv_device_config.h \
    ../ScudAdmin/sv_select_model_dialog.h \
    ../ScudAdmin/sv_dev_model.h \
    ../ScudAdmin/sv_dev_brand.h \
    ../ScudAdmin/sv_dev_class.h \
    ../ScudAdmin/sv_select_brand_dialog.h \
    ../ScudAdmin/sv_select_class_dialog.h

FORMS    += mainwindow.ui \
    sv_trackersettings.ui

RESOURCES += \
    ../res.qrc
