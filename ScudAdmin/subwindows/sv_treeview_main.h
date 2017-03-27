#ifndef SVTREEVIEWWINDOW_H
#define SVTREEVIEWWINDOW_H
#include <QAxWidget>
#include <QAxObject>

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

#include "sv_employee.h"

#include "../treeitem.h"
#include "sv_model_main.h"

#include "../../libxl-3.6.4.0\include_cpp/libxl.h"
#include "./EXCEL/excel.h"
#include "../defs_misscud.h"

class SvTreeViewMain : public QTreeView
{
  Q_OBJECT
  
  public:
    explicit SvTreeViewMain(QWidget* parent,
                            int departmentId,
                            int employeeId,
                            QDate periodBegin,
                            QDate periodEnd,
                            int reportType,
                            QMdiArea* mdiWindow = 0);
  
    ~SvTreeViewMain();
    
    SvModelMain* model;
    
    QAction *actionExpandAll;
    QAction *actionEditEmployee;
    QAction *actionReload;
    QAction* actionCreateXLS;
    
    int departmentId;    
    int employeeId;
    QDate periodBegin;
    QDate periodEnd;
    int report_type;

  private:
    QMdiArea* mdiWindow;
    
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE ;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;
    
    void setupModelData(QStringList &headers);

    void d(QAxObject *range);
    
    QDialog* dlgBusy;
    
    bool expanded = false;
    
  private slots:
    void on_actionExpandAll_triggered();
    void on_actionEditEmployee_triggered();
    void on_actionReload_triggered();
    
    void on_actionCreateXLS_triggered();
//    void on_actionAlarms_triggered();
//    void on__triggered();  
    
    void showBusy(QString text);
    
  signals:
    void wantReload();
    void reportCreated();
    
};


#endif // SVTREEVIEWWINDOW_H
