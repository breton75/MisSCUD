#ifndef SVSUBWINDOWABSENCEJOURNAL_H
#define SVSUBWINDOWABSENCEJOURNAL_H

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
#include "sv_treeview_absence_journal.h"

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"

#include "../defs_misscud.h"

#define SUBWINDOW_ABSENCE_JOURNAL_NAME "subw_absence_journal_%1_%2"

class SvSubWindowAbsenceJournal : public QWidget
{
    Q_OBJECT
  public:
    explicit SvSubWindowAbsenceJournal(QMdiArea *parent,
                             int departmentId,
                             int employeeId,
                             QDate periodBegin,
                             QDate periodEnd);
    ~SvSubWindowAbsenceJournal();
  
  private:

    int departmentId;    
    int employeeId;
    QDate periodBegin;
    QDate periodEnd;
    
    QMdiSubWindow *subw;
    
    QVBoxLayout* vlay;
    QToolBar* toolbar;

    SvTreeViewAbsenceJournal* tree;

    QString IniGroupName;
    
    void setupModelData();
    
};

#endif // SVSUBWINDOWABSENCEJOURNAL_H
