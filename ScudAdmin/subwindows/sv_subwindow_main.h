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
                             int employeeId,
                             QDate periodBegin,
                             QDate periodEnd,
                             int reportType);
    ~SvSubWindowMain();
  
  private:

    int departmentId;    
    int employeeId;
    QDate periodBegin;
    QDate periodEnd;
    int report_type;
    
    QMdiSubWindow *subw;
    
    QVBoxLayout* vlay;
    QToolBar* toolbar;

    SvTreeViewMain* tree;
        
    void setupModelData();

  public slots:
    void update();
    
};

#endif // SVSUBWINDOWMAIN_H
