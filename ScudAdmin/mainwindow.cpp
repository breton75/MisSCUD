#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sv_treeview_misscud.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"

QSqlQueryModel* qm;

SvTreeView* tree;
extern SvPGDB* PGDB;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  
  tree = new SvTreeView(ui->dockWidgetContents, ui->mdiArea);
  
  ui->menuProjects->insertAction(0, tree->actionNewDevice);
  ui->menuProjects->insertAction(0, tree->actionEditDevice);
  ui->menuProjects->insertAction(0, tree->actionDeleteDevice);
  ui->menuProjects->addSeparator();
  ui->menuProjects->insertAction(0, tree->actionReloadDevices);
  
  ui->menuTasks->insertAction(0, tree->actionNewScudKey);
  ui->menuTasks->insertAction(0, tree->actionEditScudKey);
  ui->menuTasks->insertAction(0, tree->actionDeleteScudKey);
  ui->menuTasks->addSeparator();
  ui->menuTasks->insertAction(0, tree->actionReloadScudKeys);
  
  ui->menuEmployees->insertAction(0, tree->actionNewEmployee);
  ui->menuEmployees->insertAction(0, tree->actionEditEmployee);
  ui->menuEmployees->insertAction(0, tree->actionDeleteEmployee);
  ui->menuTasks->addSeparator();
  ui->menuEmployees->insertAction(0, tree->actionReloadEmployees);
  
  ui->verticalLayout_4->addWidget(tree);
  
//  ui->mainToolBar->insertAction(0, tree->actionNewDevice);
//  ui->mainToolBar->insertAction(0, tree->actionEditDevice);
//  ui->mainToolBar->insertAction(0, tree->actionDeleteDevice);
//  ui->mainToolBar->insertSeparator(0);
  
  ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  
  ui->mainToolBar->insertSeparator(0);
  ui->mainToolBar->insertAction(0, tree->actionNewEmployee);
//  ui->mainToolBar->insertAction(0, tree->actionEditEmployee);
//  ui->mainToolBar->insertAction(0, tree->actionDeleteEmployee);
  ui->mainToolBar->insertSeparator(0);
  ui->mainToolBar->insertAction(0, tree->actionNewDepartment);
//  ui->mainToolBar->insertSeparator(0);
//  ui->mainToolBar->insertAction(0, tree->actionNewScudKey);
//  ui->mainToolBar->insertSeparator(0);
//  ui->mainToolBar->insertAction(0, tree->actionReportMain);
  
//  ui->mainToolBar->insertSeparator(0);
//  ui->mainToolBar->insertAction(0, tree->actionBusinessCalendar);  
  
//  ui->mainToolBar->insertSeparator(0);
//  ui->mainToolBar->insertAction(0, tree->actionAbsenceJournal); 

  ui->mainToolBar->insertSeparator(0);
  ui->mainToolBar->insertAction(0, tree->actionTimeSheet);   
  
  // читаем размер и положение окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  this->resize(sett->readValue("MAIN WINDOW", "Size", QSize(600, 400)).toSize());
  this->move(sett->readValue("MAIN WINDOW", "Position", QPoint(100, 100)).toPoint());
  this->setWindowState(Qt::WindowState(sett->readValue("MAIN WINDOW", "WindowState", Qt::WindowNoState).toInt()));
  sett->~SvSettings();
  
//  connect(ui->bnDuty, SIGNAL(clicked()), this, SLOT(duty()));
    
}

MainWindow::~MainWindow()
{
  // закрываем дочерние окна с сохранением параметров
  foreach (QObject* wnd, ui->mdiArea->children())
  {
    if(wnd->metaObject()->className() == "SvProjectsGeneralTable")
    {
//      SvProjectsGeneralTable* projectWindow = static_cast<SvProjectsGeneralTable*>(wnd);
//      projectWindow->~SvProjectsGeneralTable();
    }
  }

  // сохраняем параметры главного окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  if(this->windowState() != Qt::WindowNoState)
    sett->writeValue("MAIN WINDOW", "WindowState", this->windowState());

  else
  {
    sett->writeValue("MAIN WINDOW", "Size", this->size());
    sett->writeValue("MAIN WINDOW", "Position", this->pos());
    sett->writeValue("MAIN WINDOW", "WindowState", Qt::WindowNoState);
  }
  sett->~SvSettings();
  
  delete ui;
}

void MainWindow::AfterShow()
{
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  bool b1 = sett->readValue("DEBUG", "loadDevices", true).toBool();
  bool b2 = sett->readValue("DEBUG", "showLog", true).toBool();
  sett->~SvSettings();
  
  if(b1)
    tree->loadDevices();
  tree->loadEmployees();
  tree->loadJournals();
  tree->loadReports();
  
  ui->dockWidget_7->setVisible(b2);

//  tree->expand(tree->model()->index(0, 0, tree->rootIndex()));
  
  tree->setFocus();
//  tree->setCurrentIndex(tree->model()->index(0, 0, tree->rootIndex()));
}

QTextEdit* MainWindow::get_log()
{
  return ui->textLog;
}

void MainWindow::duty()
{
  QFile *f = new QFile();
  f->setFileName("D:/c++/MisSCUD/emploees_directory.csv");
  if (!f->open(QFile::ReadOnly)) 
    return;

  QStringList list = QStringList();
  QString last_department = "";
//  QSqlQuery* q = new QSqlQuery(PGDB->db);
  QByteArray b = f->readLine();
  b = f->readLine();
  while(b.size())
  {

    QString s = "";
    s.append(b);

    if(s.contains(";;;;;;;;;;;;"))
    {
      last_department = s.split(";").at(0).trimmed();

      if(QSqlError::NoError != PGDB->execSQL(QString("insert "
                                                     "into departments "
                                                     "(department_name, boss_id, description, icon) values('%1', -1, '', (select icon from departments where id = 5 limit 1))")
                                             .arg(last_department)).type())
      {
        qDebug() << "error department: " << last_department;
        return;
      }
      qDebug() << "департамент: " << last_department;
    }
    
    else
    {
      list = s.split(";");

     /* qDebug() << list.at(0).section(' ', 0, 0).trimmed() <<
          list.at(0).section(' ', 1, 1).trimmed() <<
          list.at(0).section(' ', 2, 2).trimmed() <<
          list.at(1) <<
//          last_department <<
          list.at(2) <<
          list.at(3) <<
          QString("%1; %2").arg(list.at(4)).arg(list.at(5));*/
      
      
      
      if(QSqlError::NoError != PGDB->execSQL(QString("insert into employees ("
                                                     "  employee_surename, "
                                                     "  employee_name, "
                                                     "  employee_patronymic, "
                                                     "  employee_position, "
                                                     "  department_id, "
                                                     "  phone1, "
                                                     "  phone2, "
                                                     "  email) "
                                                     "values('%1', '%2', '%3', '%4', "
                                                     "  (select id from departments where department_name = '%5' limit 1), "
                                                     "  '%6', "
                                                     "  '%7', "
                                                     "  '%8') ")
                                             .arg(list.at(0).section(' ', 0, 0).trimmed())
                                             .arg(list.at(0).section(' ', 1, 1).trimmed())
                                             .arg(list.at(0).section(' ', 2, 2).trimmed())
                                             .arg(list.at(1).trimmed())
                                             .arg(last_department)
                                             .arg(list.at(2).trimmed())
                                             .arg(list.at(3).trimmed())
                                             .arg(QString("%1; %2").arg(list.at(4).trimmed()).arg(list.at(5).trimmed()))).type())
      {
        qDebug() << "error employee: " << s;
        return; 
      }
      qDebug() << s;    
    }
    
    b = f->readLine();
          
  }
  
  f->close();
  delete f;
  qDebug() << "успешно";
}
