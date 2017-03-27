#ifndef SVSUBWINDOWTIMESHEET_H
#define SVSUBWINDOWTIMESHEET_H

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
#include "sv_tableview_timesheet.h"

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"

#include "../defs_misscud.h"

#define SUBWINDOW_TIMESHEET_NAME "subw_window_timesheet_%1_%2"

class SvSubWindowTimeSheet : public QWidget
{
    Q_OBJECT
  public:
    explicit SvSubWindowTimeSheet(QMdiArea *parent,
                             int departmentId,
                             int employeeId,
                             int month,
                             int year);
    ~SvSubWindowTimeSheet();
  
  private:

    int departmentId;    
    int employeeId;
    int month;
    int year;
    
    QMdiSubWindow *subw;
    
    QVBoxLayout* vlay;
    QToolBar* toolbar;

    SvTableViewTimeSheet* table;
    
    QString IniGroupName;
    
    QString monthNames[12] = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль",
                              "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    
    void setupModelData();

  public slots:
    void update();
    
};

#endif // SVSUBWINDOWTIMESHEET_H
