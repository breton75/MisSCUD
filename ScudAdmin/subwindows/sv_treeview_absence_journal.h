#ifndef SVTREEVIEWABSENCEJOURNAL_H
#define SVTREEVIEWABSENCEJOURNAL_H

#include <QAxWidget>
#include <QAxObject>
#include <QTextCodec>
#ifdef _WIN32
  #include <windows.h>
#endif

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
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
//#include <QPushButton>

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"

#include "sv_absence.h"

#include "../treeitem.h"
#include "sv_model_main.h"

#include "./EXCEL/excel.h"
#include "../defs_misscud.h"

class SvTreeViewAbsenceJournal : public QTreeView
{
  Q_OBJECT
  
  public:
    explicit SvTreeViewAbsenceJournal(QWidget* parent,
                            int departmentId,
                            int employeeId,
                            QDate periodBegin,
                            QDate periodEnd,
                            QMdiArea* mdiWindow = 0);
  
    ~SvTreeViewAbsenceJournal();
    
    SvModelMain* model;
    
    QAction *actionNewAbsenceJornal;
    QAction *actionEditAbsenceJornal;
    QAction *actionDeleteAbsenceJornal;
    
    QAction *actionExpandAll;
    QAction *actionReload;
    QAction* actionCreateXLS;
    
    int departmentId;    
    int employeeId;
    QDate periodBegin;
    QDate periodEnd;

  private:
    QMdiArea* mdiWindow;
    
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE ;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;
    
    void setupModelData();

//    void d(QAxObject *range);
    
    QDialog* dlgBusy;
    
    bool expanded = false;
    
  private slots:
    void on_actionNewAbsenceJornal_triggered();
    void on_actionEditAbsenceJornal_triggered();
    void on_actionDeleteAbsenceJornal_triggered();
    
    void on_actionExpandAll_triggered();
    void on_actionReload_triggered();
    
    void on_actionCreateXLS_triggered();
//    void on_actionAlarms_triggered();
//    void on__triggered();  
    
    void showBusy(QString text);
    
  signals:
    void wantReload();
    void reportCreated();
    
};


#endif // SVTREEVIEWABSENCEJOURNAL_H
