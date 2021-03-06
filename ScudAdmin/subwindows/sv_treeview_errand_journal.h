#ifndef SVTREEVIEWERRANDJOURNAL_H
#define SVTREEVIEWERRANDJOURNAL_H

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

#include "sv_errand.h"

#include "../treeitem.h"
#include "sv_model_main.h"

#include "./EXCEL/excel.h"
#include "../defs_misscud.h"

class SvTreeViewErrandJournal : public QTreeView
{
  Q_OBJECT
  
  public:
    explicit SvTreeViewErrandJournal(QWidget* parent,
                            int departmentId,
                            int employeeId,
                            QDate periodBegin,
                            QDate periodEnd,
                            QMdiArea* mdiWindow = 0);
  
    ~SvTreeViewErrandJournal();
    
    SvModelMain* model;
    
    QAction *actionNewErrandJornal;
    QAction *actionEditErrandJornal;
    QAction *actionDeleteErrandJornal;
    
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
    void on_actionNewErrandJornal_triggered();
    void on_actionEditErrandJornal_triggered();
    void on_actionDeleteErrandJornal_triggered();
    
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


#endif // SVTREEVIEWERRANDJOURNAL_H
