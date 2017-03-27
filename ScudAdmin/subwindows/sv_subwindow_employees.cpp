#include "sv_subwindow_main.h"

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;

QString IniGroupName;

SvSubWindowMain* SUBWINDOW_MAIN_UI;

SvSubWindowMain::SvSubWindowMain(QMdiArea *parent,
                                 int departmentId, QString departmentName,
                                 int employeeId, QString employeeName,
                                 QDateTime periodBegin,
                                 QDateTime periodEnd) : 
  QWidget(parent)
{
  this->setObjectName(QStringLiteral(SUBWINDOW_MAIN_NAME).arg(departmentId).arg(employeeId));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  this->departmentId = departmentId;
  this->employeeId = employeeId;
  this->periodBegin = periodBegin;
  this->periodEnd = periodEnd;  
  
  // задаем настройки для ОКНА !
  subw = parent->addSubWindow(this);
  subw->setWindowIcon(QIcon(":/munich/icons/munich-icons/ico/blue/report_employee.ico"));
  subw->setAttribute(Qt::WA_DeleteOnClose);
  
  // читаем размер и положение окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  IniGroupName = QString("REPORT MAIN %1 %2").arg(departmentId).arg(employeeId);
  subw->resize(sett->readValue(IniGroupName, "Size", QSize(400, 200)).toSize());
  subw->move(sett->readValue(IniGroupName, "Position", QPoint(30, 30)).toPoint());
  subw->setWindowState(Qt::WindowState(sett->readValue(IniGroupName, "WindowState", Qt::WindowNoState).toInt()));
 
  /** создаем верхнюю панель **/
  vlay = new QVBoxLayout(this);
  vlay->setParent(this);
  vlay->setSpacing(2);
  vlay->setContentsMargins(2, 2, 2, 2);
  vlay->setObjectName(QStringLiteral("vlay"));

//  return;
  tree = new SvTreeViewMain(this, departmentId, employeeId, periodBegin, periodEnd, parent);
  connect(tree->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateProjSum(QModelIndex,QModelIndex)));
  
  toolbar = new QToolBar(this);
  toolbar->insertAction(0, tree->actionLoadDetails);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, tree->actionEditEmployee);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, tree->actionCreateXLS);
  
  
  QString lbl = "";
  lbl = QString("Период: %1 - %2")
      .arg(this->periodBegin.date().toString("dd.MM.yyyy"))
      .arg(this->periodEnd.date().toString("dd.MM.yyyy"));
  
  
  lblPeriod = new QLabel(lbl, toolbar);
  lblPeriod->setObjectName("lblPeriod");
  lblPeriod->setMinimumWidth(200);
  lblPeriod->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  lblPeriod->setAlignment(Qt::AlignRight);
  
  lblPeriod->resize(200, 24);
    
  lblEmployee = new QLabel(toolbar);
  lblEmployee->setObjectName("lblEmployee");
  lblEmployee->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lblEmployee->setAlignment(Qt::AlignRight);
  lblEmployee->resize(200, 24);
  
  if((departmentId == -1) && (employeeId == -1))
    lbl = "Все сотрудники";
  else if(employeeId == -1)
    lbl = "Отдел: " + departmentName;
  else
    lbl = "Сотрудник: " + employeeName;

  lblEmployee->setText(lbl);

  toolbar->insertWidget(0, lblPeriod);
  toolbar->insertWidget(0, lblEmployee);

//  toolbar->insertAction(0, tree->action);
  
  
//  panel.createTopPanel(this);
  
  vlay->addWidget(toolbar);
//  vlay->addWidget(panel.frame);
  vlay->addWidget(tree);

  /** создаем нижнюю панель проекта **/
//  panel.createBottomPanel(this);
//  vlay->addWidget(panel.frame2);
  
//  setProjectPanelsData();

}

SvSubWindowMain::~SvSubWindowMain()
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


void SvSubWindowMain::update()
{
//  setTabelData();
}

void SvSubWindowMain::setProjectPanelsData()
{
//  QSqlQuery* q = new QSqlQuery(PGDB->db);
//  QSqlError err;

  
  
//  // выбираем информацию по проекту и заполняем поля панели
//  err = PGDB->execSQL(QString("select * from projects where id = %1").arg(projectId), q);
//  q->first();
  
//  this->projectName = q->value("project_name").toString();
//  this->projectCode = q->value("project_code").toString();
//  this->projectState = q->value("state").toInt();
//  this->projectBegin = q->value("project_begin").toDate();
//  this->projectEnd = q->value("project_end").toDate();
//  this->projectLaboriounessPlan = q->value("project_plan_laboriousness").toFloat();

//  q->finish();
  
//  QIcon ico;
//  switch (projectState)
//  {
//    case ProjectNew:
//      ico.addFile(":/munich/icons/munich-icons/ico/blue/project_kp.ico");
//      break;
      
//    case ProjectOnWork:
//      ico.addFile(":/munich/icons/munich-icons/ico/blue/project_onwork.ico");
//      break;
      
//    case ProjectDone:
//      ico.addFile(":/tree/icons/GradientOk.ico");
//      break;  
//  }
//  this->setWindowTitle(QString("Отчет: Занятость сотрудников по проекту %1").arg(projectName));
//  panel.lblStateImg->setPixmap(QPixmap(ico.pixmap(16, 16)));
  
//  panel.editProjectName->setText(projectName);
//  panel.editProjectCode->setText(projectCode);
//  panel.editProjectBegin->setText(projectBegin.toString("dd.MM.yyyy"));
//  panel.editProjectEnd->setText(projectEnd.toString("dd.MM.yyyy"));
  
//  /* 3. Дней до завершения */
//  QDate today = QDate::currentDate();
//  if(today > projectEnd)
//    panel.editDaysUntilEnd->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 207);\n"
//                                                        "color: rgb(255, 0, 0);"));
//  else
//    panel.editDaysUntilEnd->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 207);\n"
//                                                        "color: rgb(0, 0, 255);"));
    
//    panel.editDaysUntilEnd->setText(QString::number(today.daysTo(projectEnd)));
    
//  panel.bnPlanFact->setVisible(false);
//  panel.bnFactUpdate->setVisible(false);
    
    
//  panel.editProjectLaboriousnessPlan->setText(QString::number(projectLaboriounessPlan));
  
  
//  /** *** вычисляем вычисляемые значения *** **/
  
//  /* 1. общая сумма трудозатрат по проекту */
//  err = PGDB->execSQL("select login from employees", q);
//  QString s = "";
//  while(q->next())
//    s += (s == "" ? "select sum(" : " + ") + q->value("login").toString();
//  q->finish();
  
//  s += QString(") as project_sum from general where task_id in (select tasks.id from tasks where tasks.project_id = %1)").arg(projectId);
    
//  err = PGDB->execSQL(s, q);
//  q->first();
  
//  this->projectLaboriounessCurrent = q->value("project_sum").toFloat();
//  panel.editProjectLaboriousnessCurrent->setText(QString::number(projectLaboriounessCurrent));
  
//  q->finish();    
//  free(q);
  
//  /* 2. разница между плановыми и текущими трудозатратами */
//  if(projectLaboriounessPlan >= projectLaboriounessCurrent)
//    panel.editProjectLaboriousnessDifference->setStyleSheet(QLatin1String("background-color: rgb(85, 255, 127);\n"
//                                                                          "alternate-background-color: rgb(85, 255, 127);"));
//  else 
//    panel.editProjectLaboriousnessDifference->setStyleSheet(QLatin1String("background-color: rgb(255, 129, 129);\n"
//                                                                          "alternate-background-color: rgb(255, 129, 129);"));
  
//  panel.editProjectLaboriousnessDifference->setText(QString::number(projectLaboriounessPlan - projectLaboriounessCurrent));
  
//  
  
}


