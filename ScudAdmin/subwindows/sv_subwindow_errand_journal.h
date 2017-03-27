#ifndef SVSUBWINDOWERRANDJOURNAL_H
#define SVSUBWINDOWERRANDJOURNAL_H

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
//#include "sv_treeview_main.h"
#include "sv_treeview_errand_journal.h"

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"

#include "../defs_misscud.h"

#define SUBWINDOW_ERRAND_JOURNAL_NAME "subw_errand_journal_%1_%2"

class SvSubWindowErrandJournal : public QWidget
{
    Q_OBJECT
  public:
    explicit SvSubWindowErrandJournal(QMdiArea *parent,
                             int departmentId,
                             int employeeId,
                             QDate periodBegin,
                             QDate periodEnd);
    ~SvSubWindowErrandJournal();
  
  private:

    int departmentId;    
    int employeeId;
    QDate periodBegin;
    QDate periodEnd;
    int report_type;
    
    QMdiSubWindow *subw;
    
    QVBoxLayout* vlay;
    QToolBar* toolbar;

    SvTreeViewErrandJournal* tree;

    QString IniGroupName;
    
    void setupModelData();

  public slots:
    void update();
    
};

#endif // SVSUBWINDOWERRANDJOURNAL_H
