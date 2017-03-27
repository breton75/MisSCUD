#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T11:37:15
#
#-------------------------------------------------

QT       += core gui sql network axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#ICON += ../icons/cologne-icons/ico_original/old-versions.ico
RC_FILE = ScudAdmin.rc

VERSION = 2.0.1.23

TARGET = ScudAdmin
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    sv_treeview_misscud.cpp \
    sv_dev_brand.cpp \
    sv_dev_class.cpp \
    sv_dev_model.cpp \
    sv_device.cpp \
    sv_select_brand_dialog.cpp \
    sv_select_class_dialog.cpp \
    sv_select_model_dialog.cpp \
    ../../Common/log.cpp \
    ../../Common/sv_pgdb.cpp \
    ../../Common/sv_settings.cpp \
    ../../Common/sv_userlogon.cpp \
    sv_employee.cpp \
    sv_scud.cpp \
    ../../Common/sv_tcpserverclient.cpp \
    ../../Common/sv_secondmeter.cpp \
    sv_device_config.cpp \
    sv_department.cpp \
    sv_select_department_dialog.cpp \
    sv_select_scudkey_dialog.cpp \
    ../treeitem.cpp \
    subwindows/sv_model_main.cpp \
    subwindows/sv_subwindow_main.cpp \
    subwindows/sv_treeview_main.cpp \
    sv_report_settings_dialog.cpp \
    EXCEL/excel.cpp \
    subwindows/sv_subwindow_calendar.cpp \
    sv_select_holiday_dialog.cpp \
    sv_holiday.cpp \
    sv_absence.cpp \
    subwindows/sv_subwindow_absence_journal.cpp \
    subwindows/sv_treeview_absence_journal.cpp \
    subwindows/sv_subwindow_timesheet.cpp \
    subwindows/sv_tableview_timesheet.cpp \
    subwindows/sv_subwindow_errand_journal.cpp \
    subwindows/sv_treeview_errand_journal.cpp \
    sv_errand.cpp

HEADERS  += mainwindow.h \
    sv_treeview_misscud.h \
    sv_dev_brand.h \
    sv_dev_class.h \
    sv_dev_model.h \
    sv_device.h \
    sv_select_brand_dialog.h \
    sv_select_class_dialog.h \
    sv_select_model_dialog.h \
    ../../Common/log.h \
    ../../Common/sv_pgdb.h \
    ../../Common/sv_settings.h \
    ../../Common/sv_userlogon.h \
    sv_employee.h \
    sv_scud.h \
    ../../Common/sv_tcpserverclient.h \
    ../../Common/sv_secondmeter.h \
    sv_device_config.h \
    sv_department.h \
    sv_select_department_dialog.h \
    sv_select_scudkey_dialog.h \
    ../treeitem.h \
    subwindows/sv_model_main.h \
    subwindows/sv_subwindow_main.h \
    subwindows/sv_treeview_main.h \
    sv_report_settings_dialog.h \
    EXCEL/excel.h \
    subwindows/sv_subwindow_calendar.h \
    sv_select_holiday_dialog.h \
    sv_holiday.h \
    sv_absence.h \
    subwindows/sv_subwindow_absence_journal.h \
    subwindows/sv_treeview_absence_journal.h \
    subwindows/sv_subwindow_timesheet.h \
    subwindows/sv_tableview_timesheet.h \
    subwindows/sv_subwindow_errand_journal.h \
    subwindows/sv_treeview_errand_journal.h \
    sv_errand.h

FORMS    += mainwindow.ui \
    sv_employee.ui \
    sv_scud.ui \
    sv_deviceclass.ui \
    report_settings.ui \
    sv_holiday.ui \
    sv_absence.ui \
    forms/sv_errand.ui

RESOURCES += \
    ../res.qrc
