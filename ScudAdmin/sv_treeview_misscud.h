#ifndef SVTREEVIEW_H
#define SVTREEVIEW_H

#include <QTreeView>
#include <QWidget>
#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QMdiArea>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressDialog>
#include <QtEndian>
#include <QVBoxLayout>
#include <QMdiSubWindow>

#include <QStringBuilder>
#include <QCalendarWidget>

//#include <QComboBox>
//#include <QLabel>
//#include <QPushButton>

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"
#include "sv_employee.h"
#include "sv_device.h"
#include "sv_department.h"
#include "./subwindows/sv_subwindow_main.h"
#include "sv_report_settings_dialog.h"
#include "./subwindows/sv_subwindow_calendar.h"
#include "./subwindows/sv_subwindow_absence_journal.h"
#include "./subwindows/sv_subwindow_errand_journal.h"
#include "./subwindows/sv_subwindow_timesheet.h"

#include "../defs_misscud.h"

class SvTreeView : public QTreeView
{
  Q_OBJECT
  
  public:
    explicit SvTreeView(QWidget* parent, QMdiArea* mdiWindow = 0);
    
    ~SvTreeView();
    
    QStandardItemModel* model = new QStandardItemModel;
    
    //defining items
    QStandardItem *itemRoot = model->invisibleRootItem();
    SvDevicesItem *itemDevices; // =  new QStandardItem("Устройства");
    SvDeviceClassItem *itemClass;
    SvEmployeesItem *itemEmployees; // =  new QStandardItem("РЛС");
    SvScudKeys * itemScudKeys;
    SvReportsItem *itemReports;
    SvTimeAccountingItem *itemTimeAccounting;
    SvAbsenceJournalItem *itemAbsenceJournal;
    SvErrandJournalItem *itemErrandJournal;
    SvBusinessCalendarItem *itemBusinessCalendar;
    SvTimeSheetItem *itemTimeSheetItem;
    
//    QAction *actionDevices;
    QAction *actionNewDevice;
    QAction *actionEditDevice;
    QAction *actionDeleteDevice;  
    QAction *actionReloadDevices;
    
    QAction *actionNewDepartment;
    QAction *actionEditDepartment;
    QAction *actionDeleteDepartment;
    
    QAction *actionNewEmployee;
    QAction *actionEditEmployee;
    QAction *actionDeleteEmployee;
    QAction *actionReloadEmployees;

    QAction *actionScudKeys;
    QAction *actionNewScudKey;
    QAction *actionEditScudKey;
    QAction *actionDeleteScudKey;
    QAction *actionReloadScudKeys;
    
    QAction *actionReportMain;
    
    QAction *actionBusinessCalendar;
    QAction *actionAbsenceJournal;
    QAction *actionErrandJournal;
    QAction *actionTimeSheet;
    
//    QAction* actionReportTasksByEmployee;
    
    QModelIndex selected() { return selectedIndexes().at(0); }
    
    void loadDevices(bool reload = false);
    void loadEmployees(bool reload = false);
    void loadScudKeys(bool reload = false);
    void loadJournals(bool reload = false);
    void loadReports(bool reload = false);
    
  private:
    QMdiArea* mdiWindow;
    
    QFont fontBold;
    
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE ;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
//    void dragLeaveEvent(QDragLeaveEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;
//    QList<QAction*>* actionList = new QList<QAction*>;

  private slots:
//    void on_actionDevices_triggered();
    void on_actionNewDevice_triggered();
    void on_actionEditDevice_triggered();
    void on_actionDeleteDevice_triggered(); 
    void on_actionReloadDevices_triggered();
    
    void on_actionNewDepartment_triggered();
    void on_actionEditDepartment_triggered();
    void on_actionDeleteDepartment_triggered();
    
    void on_actionNewEmployee_triggered();
    void on_actionEditEmployee_triggered();
    void on_actionDeleteEmployee_triggered();
    void on_actionReloadEmployees_triggered();
    
    void on_actionScudKeys_triggered();
    void on_actionNewScudKey_triggered();
    void on_actionEditScudKey_triggered();
    void on_actionDeleteScudKey_triggered();
    void on_actionReloadScudKeys_triggered();
    
//    void createReport(int reportType);
    void on_actionReportMain_triggered(int reportType = -1);
    void on_actionBusinessCalendar_triggered();
    void on_actionAbsenceJournal_triggered();
    void on_actionErrandJournal_triggered();
    void on_actionTimeSheet_triggered();
    
    
//    void on_actionReportTasksByEmployee_triggered();

//    void on__triggered();    

    
};


#endif // SVTREEVIEW_H
