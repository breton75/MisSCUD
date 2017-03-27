#include "sv_subwindow_timesheet.h"

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;

SvSubWindowTimeSheet* SUBWINDOW_TIMESHEET_UI;

SvSubWindowTimeSheet::SvSubWindowTimeSheet(QMdiArea *parent,
                                 int departmentId,
                                 int employeeId,
                                 int month,
                                 int year) : 
  QWidget(parent)
{
  this->setObjectName(QStringLiteral(SUBWINDOW_TIMESHEET_NAME).arg(departmentId).arg(employeeId));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  this->departmentId = departmentId;
  this->employeeId = employeeId;
  this->month = month;
  this->year = year;
  
  // задаем настройки для ОКНА !
  subw = parent->addSubWindow(this);
  subw->setWindowIcon(QIcon(":/reports/icons/cologne-icons/ico/timesheet.ico"));
  subw->setAttribute(Qt::WA_DeleteOnClose);
  
  // читаем размер и положение окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  IniGroupName = QString("REPORT TIMESHEET %1 %2").arg(departmentId).arg(employeeId);
  subw->resize(sett->readValue(IniGroupName, "Size", QSize(400, 200)).toSize());
  subw->move(sett->readValue(IniGroupName, "Position", QPoint(30, 30)).toPoint());
  subw->setWindowState(Qt::WindowState(sett->readValue(IniGroupName, "WindowState", Qt::WindowNoState).toInt()));
 
  /** создаем верхнюю панель **/
  vlay = new QVBoxLayout(this);
  vlay->setParent(this);
  vlay->setSpacing(2);
  vlay->setContentsMargins(2, 2, 2, 2);
  vlay->setObjectName(QStringLiteral("vlay"));
 
  table = new SvTableViewTimeSheet(this, departmentId, employeeId, month, year, parent);
//  connect(tree->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateProjSum(QModelIndex,QModelIndex)));
  
  toolbar = new QToolBar(this);
  toolbar->insertAction(0, table->actionAbsenceJournal);
  toolbar->insertAction(0, table->actionErrandJournal);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, table->actionCreateXLS);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, table->actionReload);
  
  vlay->addWidget(toolbar);
  vlay->addWidget(table);

}

SvSubWindowTimeSheet::~SvSubWindowTimeSheet()
{
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";

  SvSettings *sett = new SvSettings(s, this);
  if(subw->windowState() != Qt::WindowNoState)
  {
    sett->writeValue(IniGroupName, "WindowState", subw->windowState());
  }
  else
  {
    sett->writeValue(IniGroupName, "Size", subw->size());
    sett->writeValue(IniGroupName, "Position", subw->pos());
    sett->writeValue(IniGroupName, "WindowState", Qt::WindowNoState);
  }

  QString cw = "";
  for(int i = 0; i < table->model->columnCount(); i++)
    cw += ";" + QString::number(table->columnWidth(i));
  
  sett->writeValue(IniGroupName, "Column Widths", cw.remove(0, 1));
  
  sett->~SvSettings();
  
  this->deleteLater();
}


void SvSubWindowTimeSheet::update()
{
//  setTabelData();
}



