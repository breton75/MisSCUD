#ifndef SVSUBWINDOWMAIN_H
#define SVSUBWINDOWMAIN_H

#include <QDebug>
#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QStandardItemModel>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QTreeView>
#include <QLineEdit>
//#include <QWindow>
#include <QMdiSubWindow>
#include <QApplication>
#include <QToolBar>

//#include "sv_treeview_ti.h"
#include "../treeitem.h"
#include "sv_model_main.h"
//#include "sv_projectpanelwidget.h"
#include "sv_treeview_main.h"

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"

#include "../defs_misscud.h"

#define SUBWINDOW_MAIN_NAME "subw_window_main_%1_%2"

class SvSubWindowMain : public QWidget
{
    Q_OBJECT
  public:
    explicit SvSubWindowMain(QMdiArea *parent,
                             int departmentId,
                             QString departmentName,
                             int employeeId,
                             QString employeeName,
                             QDateTime periodBegin,
                             QDateTime periodEnd);
    ~SvSubWindowMain();
  
  private:

    int departmentId;    
    int employeeId;
    QDateTime periodBegin;
    QDateTime periodEnd;

//QString projectName;
//QString projectCode;
//    int projectState;
//  QDate projectBegin;
//  QDate projectEnd;
//  float projectLaboriounessPlan;
//  float projectLaboriounessCurrent;
    
    QLabel* lblPeriod;
    QLabel* lblEmployee;
    
    QMdiSubWindow *subw;
    
    QVBoxLayout* vlay;
    QToolBar* toolbar;
//    SvProjectPanelWidget panel;

    SvTreeViewMain* tree;
//    QTreeView *tree;
//    SvReportEmployeeByProjectModel* model;
//    SvTaskImplementationModel* model;
        
    void setupModelData();
    void setProjectPanelsData();
    
  signals:
    
  public slots:
    void update();
//    void updateProjSum(QModelIndex topLeft, QModelIndex bottomRight);
    
};

#endif // SVSUBWINDOWMAIN_H
