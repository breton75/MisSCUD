#include "sv_subwindow_absence_journal.h"

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;

SvSubWindowAbsenceJournal* SUBWINDOW_ABSENCE_JOURNAL_UI;

SvSubWindowAbsenceJournal::SvSubWindowAbsenceJournal(QMdiArea *parent,
                                 int departmentId,
                                 int employeeId,
                                 QDate periodBegin,
                                 QDate periodEnd) : 
  QWidget(parent)
{
  this->setObjectName(QStringLiteral(SUBWINDOW_ABSENCE_JOURNAL_NAME).arg(departmentId).arg(employeeId));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->setWindowTitle("Журнал отсутствия на работе");
  
  this->departmentId = departmentId;
  this->employeeId = employeeId;
  this->periodBegin = periodBegin;
  this->periodEnd = periodEnd;  
  
  // задаем настройки для ОКНА !
  subw = parent->addSubWindow(this);
  subw->setWindowIcon(QIcon(":/tree/icons/cologne-icons/ico/absence_journal.ico"));
  subw->setAttribute(Qt::WA_DeleteOnClose);
  
  // читаем размер и положение окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  IniGroupName = QString("ABSENCE JOURNAL %1 %2").arg(departmentId).arg(employeeId);
  subw->resize(sett->readValue(IniGroupName, "Size", QSize(400, 200)).toSize());
  subw->move(sett->readValue(IniGroupName, "Position", QPoint(30, 30)).toPoint());
  subw->setWindowState(Qt::WindowState(sett->readValue(IniGroupName, "WindowState", Qt::WindowNoState).toInt()));
 
  /** создаем верхнюю панель **/
  vlay = new QVBoxLayout(this);
  vlay->setParent(this);
  vlay->setSpacing(2);
  vlay->setContentsMargins(2, 2, 2, 2);
  vlay->setObjectName(QStringLiteral("vlay"));

  tree = new SvTreeViewAbsenceJournal(this, departmentId, employeeId, periodBegin, periodEnd, parent);
//  connect(tree->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateProjSum(QModelIndex,QModelIndex)));
  
  toolbar = new QToolBar(this);
  toolbar->insertAction(0, tree->actionNewAbsenceJornal);
  toolbar->insertAction(0, tree->actionEditAbsenceJornal);
  toolbar->insertAction(0, tree->actionDeleteAbsenceJornal);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, tree->actionExpandAll);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, tree->actionCreateXLS);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, tree->actionReload);
  
  vlay->addWidget(toolbar);
  vlay->addWidget(tree);

}

SvSubWindowAbsenceJournal::~SvSubWindowAbsenceJournal()
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
  for(int i = 0; i < tree->model->columnCount(); i++)
    cw += ";" + QString::number(tree->columnWidth(i));
  
  sett->writeValue(IniGroupName, "Column Widths", cw.remove(0, 1));
  
  sett->~SvSettings();
  
  this->deleteLater();
}




