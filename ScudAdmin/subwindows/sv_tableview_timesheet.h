#ifndef SVTABLEVIEWTIMESHEET_H
#define SVTABLEVIEWTIMESHEET_H

#include <QAxWidget>
#include <QAxObject>
#include <QTextCodec>
#ifdef _WIN32
  #include <windows.h>
#endif

#include <QTreeView>
#include <QTableView>
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
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
//#include <QPushButton>

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"

#include "../treeitem.h"
#include "sv_model_main.h"
#include "sv_subwindow_main.h"
#include "./subwindows/sv_subwindow_absence_journal.h"
#include "./subwindows/sv_subwindow_errand_journal.h"

#include "./EXCEL/excel.h"
#include "../defs_misscud.h"

class SvTableViewTimeSheet : public QTableView
{
  Q_OBJECT
  
  public:
    explicit SvTableViewTimeSheet(QWidget* parent,
                            int departmentId,
                            int employeeId,
                            int month,
                            int year,
                            QMdiArea* mdiWindow = 0);
  
    ~SvTableViewTimeSheet();
    
    SvModelMain* model;
    
    QAction *actionReload;
    QAction* actionCreateXLS;
    
    QAction *actionAbsenceJournal;
    QAction *actionErrandJournal;
    
    int departmentId;    
    int employeeId;
    int month;
    int year;
//    QDate periodBegin;
//    QDate periodEnd;

  private:
    QMdiArea* mdiWindow;
    
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE ;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;
    
    void setupModelData();
    
    QDialog* dlgBusy;
    
    bool expanded = false;
    
  private slots:
    void on_actionReload_triggered();
    
    void on_actionCreateXLS_triggered();
    void on_actionAbsenceJournal_triggered();
    void on_actionErrandJournal_triggered();
//    void on_actionAlarms_triggered();
//    void on__triggered();  
    
    void showBusy(QString text);
    
  signals:
    void reportCreated();
    
};


#endif // SVTABLEVIEWTIMESHEET_H
