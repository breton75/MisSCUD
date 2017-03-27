#include "sv_treeview_misscud.h"

using namespace log_ns;

extern SvPGDB* PGDB;
extern SvEmployee* EMPLOYEE_UI;
extern SvDevice* DEVICE_UI;
extern SvDepartment* DEPARTMENT_UI;
extern SvSubWindowMain* SUBWINDOW_MAIN_UI;
extern SvReportSettingsDialog* REPORTSETTINGS_UI;
extern SvSubWindowCalendar* SUBWINDOW_CALENDAR;
extern SvSubWindowAbsenceJournal* SUBWINDOW_ABSENCE_JOURNAL_UI;
extern SvSubWindowErrandJournal* SUBWINDOW_ERRAND_JOURNAL_UI;
extern SvSubWindowTimeSheet* SUBWINDOW_TIMESHEET_UI;

//extern SvProjectsGeneralTable* GENERAL_WINDOW_UI;
//extern SvReportEmployeeByProject* REPORT_EMPLOYEE_WINDOW_UI;

SvTreeView::SvTreeView(QWidget *parent, QMdiArea *mdiWindow)
{
  this->setParent(parent);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  this->setHeaderHidden(true);
  this->mdiWindow = mdiWindow;

  this->setModel(model);
  this->setVisible(true);
  
  fontBold.setBold(true);
  
  QIcon icon;
  
  /** *********** actions ************** **/
//  icon.addFile(QStringLiteral(":/munich/icons/munich-icons/ico/blue/project.ico"), QSize(), QIcon::Normal, QIcon::Off);
//  actionDevices = new QAction(this);
//  actionDevices->setObjectName(QStringLiteral("actionDevices"));
//  actionDevices->setIcon(icon);
//  actionDevices->setText("Открыть устройства");
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/plus.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionNewDevice = new QAction(this);
  actionNewDevice->setObjectName(QStringLiteral("actionNewDevice"));
  actionNewDevice->setIcon(icon);
  actionNewDevice->setText("Новое устройство");
  
  actionEditDevice = new QAction(this);
  actionEditDevice->setObjectName(QStringLiteral("actionEditDevice"));
  actionEditDevice->setText("Редактировать");
//  icon.addFile(QStringLiteral(":/office16/icons/pretty16/briefcase_info16.ico"), QSize(), QIcon::Normal, QIcon::Off);
//  actionEditDevice->setIcon(icon);

  actionDeleteDevice = new QAction(this);
  actionDeleteDevice->setObjectName(QStringLiteral("actionDeleteDevice"));
  actionDeleteDevice->setText("Удалить");
//  icon.addFile(QStringLiteral(":/office16/icons/pretty16/briefcase_delete16.ico"), QSize(), QIcon::Normal, QIcon::Off);
//  actionDeleteDevice->setIcon(icon);

  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReloadDevices = new QAction(this);
  actionReloadDevices->setObjectName(QStringLiteral("actionReloadDevices"));
  actionReloadDevices->setIcon(icon);
  actionReloadDevices->setText("Обновить");
  

  icon.addFile(QStringLiteral(":/munich/icons/munich-icons/ico/blue/task.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionScudKeys = new QAction(this);
  actionScudKeys->setObjectName(QStringLiteral("actionScudKeys"));
  actionScudKeys->setIcon(icon);
  actionScudKeys->setText("Открыть список ключей");
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/new_key.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionNewScudKey = new QAction(this);
  actionNewScudKey->setObjectName(QStringLiteral("actionNewScudKey"));
  actionNewScudKey->setIcon(icon);
  actionNewScudKey->setText("Новый ключ СКУД");
  
//  icon.addFile(QStringLiteral(":/office16/icons/pretty16/ticket_info_16.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditScudKey = new QAction(this);
  actionEditScudKey->setObjectName(QStringLiteral("actionEditScudKey"));
//  actionEditScudKey->setIcon(icon);
  actionEditScudKey->setText("Редактировать");
  
//  icon.addFile(QStringLiteral(":/office16/icons/pretty16/ticket_delete16.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionDeleteScudKey = new QAction(this);
  actionDeleteScudKey->setObjectName(QStringLiteral("actionDeleteScudKey"));
//  actionDeleteScudKey->setIcon(icon);
  actionDeleteScudKey->setText("Удалить");
  
  icon.addFile(QStringLiteral(":/munich/icons/munich-icons/ico/blue/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReloadScudKeys = new QAction(this);
  actionReloadScudKeys->setObjectName(QStringLiteral("actionReloadScudKeys"));
  actionReloadScudKeys->setIcon(icon);
  actionReloadScudKeys->setText("Обновить");
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/plus.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionNewDepartment = new QAction(this);
  actionNewDepartment->setObjectName(QStringLiteral("actionNewDepartment"));
  actionNewDepartment->setIcon(icon);
  actionNewDepartment->setText("Новый отдел");
  
//  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/plus.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditDepartment = new QAction(this);
  actionEditDepartment->setObjectName(QStringLiteral("actionEditDepartment"));
//  actionEditDepartment->setIcon(icon);
  actionEditDepartment->setText("Редактировать");
  
//  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/document-library.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionDeleteDepartment = new QAction(this);
  actionDeleteDepartment->setObjectName(QStringLiteral("actionDeleteDepartment"));
//  actionDeleteDepartment->setIcon(icon);
  actionDeleteDepartment->setText("Список сотрудников отдела");
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/new_user.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionNewEmployee = new QAction(this);
  actionNewEmployee->setObjectName(QStringLiteral("actionNewEmployee"));
  actionNewEmployee->setIcon(icon);
  actionNewEmployee->setText("Новый сотрудник");
  
//  icon.addFile(QStringLiteral(":/tree32/icons/tree32/document.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditEmployee = new QAction(this);
  actionEditEmployee->setObjectName(QStringLiteral("actionEditEmployee"));
//  actionEditEmployee->setIcon(icon);
  actionEditEmployee->setText("Редактировать");
  
//  icon.addFile(QStringLiteral(":/tree32/icons/tree32/cross.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionDeleteEmployee = new QAction(this);
  actionDeleteEmployee->setObjectName(QStringLiteral("actionDeleteEmployee"));
//  actionDeleteEmployee->setIcon(icon);
  actionDeleteEmployee->setText("Удалить сотрудника"); 
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReloadEmployees = new QAction(this);
  actionReloadEmployees->setObjectName(QStringLiteral("actionReloadEmployees"));
  actionReloadEmployees->setIcon(icon);
  actionReloadEmployees->setText("Обновить"); 

  icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/old-versions.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReportMain = new QAction(this);
  actionReportMain->setObjectName(QStringLiteral("actionReportMain"));
  actionReportMain->setIcon(icon);
  actionReportMain->setText("Отчеты");
  actionReportMain->setToolTip("Сформировать отчет");
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/date.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionBusinessCalendar = new QAction(this);
  actionBusinessCalendar->setObjectName(QStringLiteral("actionBusinessCalendar"));
  actionBusinessCalendar->setIcon(icon);
  actionBusinessCalendar->setText("Календарь");
  actionBusinessCalendar->setToolTip("Производственный календарь");  
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/absence_journal.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionAbsenceJournal = new QAction(this);
  actionAbsenceJournal->setObjectName(QStringLiteral("actionAbsenceJournal"));
  actionAbsenceJournal->setIcon(icon);
  actionAbsenceJournal->setText("Журнал отсутствия");
  actionAbsenceJournal->setToolTip("Журнал отсутствия на работе"); 
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/future-projects.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionErrandJournal = new QAction(this);
  actionErrandJournal->setObjectName(QStringLiteral("actionErrandJournal"));
  actionErrandJournal->setIcon(icon);
  actionErrandJournal->setText("Журнал местных командировок");
  actionErrandJournal->setToolTip("Журнал местных командировок"); 
  
  icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/timesheet.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionTimeSheet = new QAction(this);
  actionTimeSheet->setObjectName(QStringLiteral("actionTimeSheet"));
  actionTimeSheet->setIcon(icon);
  actionTimeSheet->setText("Табель");
  actionTimeSheet->setToolTip("Табель учета рабочего времени"); 
  
  QMetaObject::connectSlotsByName(this);
  
}

SvTreeView::~SvTreeView()
{
  this->deleteLater();
}

void SvTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu* menu = new QMenu(this);
  
  switch (this->model->itemFromIndex(this->currentIndex())->type())
  {
    case itDevices:
    case itDeviceClass:  
    {
      menu->insertAction(0, actionNewDevice);
      menu->addSeparator();
      menu->insertAction(0, actionReloadDevices);
      menu->setTitle("Устройства");
      break;
    }
      
    case itDevice:
    case itScud:
    {
//      menu->insertAction(0, actionNewDevice);
//      menu->addSeparator();
      menu->insertAction(0, actionEditDevice);
      menu->insertAction(0, actionDeleteDevice);
      menu->addSeparator();
      menu->insertAction(0, actionReloadDevices);
      menu->setTitle("Устройства");
      break;
    }
      
    case itDepartment:
    {
      menu->insertAction(0, actionNewDepartment);
      menu->insertAction(0, actionEditDepartment);
      menu->insertAction(0, actionDeleteDepartment);
      menu->addSeparator();
      menu->insertAction(0, actionReloadEmployees);
      menu->setTitle("Отделы");
      break;
    }  
      
    case itScudKeys:
    {
      menu->insertAction(0, actionScudKeys);
      menu->addSeparator();
      menu->insertAction(0, actionNewScudKey);
      menu->addSeparator();
      menu->insertAction(0, actionReloadScudKeys);
      menu->setTitle("Ключи СКУД");
      break;
    }  
      
    case itScudKey:
    {
//      menu->insertAction(0, actionNewTask);
      menu->insertAction(0, actionEditScudKey);
      menu->insertAction(0, actionDeleteScudKey);
      menu->addSeparator();
      menu->insertAction(0, actionReloadScudKeys);
      menu->setTitle("Ключи СКУД");
      break;
    }       
      
    case itEmployees:
    {
      
      menu->insertAction(0, actionNewDepartment);
      menu->addSeparator();
      menu->insertAction(0, actionNewEmployee);
      menu->addSeparator();
      menu->insertAction(0, actionReloadEmployees);
      menu->addSeparator();
      menu->insertAction(0, actionReportMain);
      menu->setTitle("Сотрудники");
      break;
    }
      
    case itEmployee:
    {
//      menu->insertAction(0, actionNewEmployee);
      menu->insertAction(0, actionEditEmployee);
      menu->insertAction(0, actionDeleteEmployee);
      menu->addSeparator();
      menu->insertAction(0, actionReloadEmployees);
      menu->setTitle("Сотрудники");
      break;
    } 
  }
  
  menu->exec(event->globalPos());
}

void SvTreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
  QStandardItem* item = this->model->itemFromIndex(this->selected());
  
  switch (item->type())
  {
    case itEmployee:
    {
      on_actionEditEmployee_triggered();
      break;
    }
      
    case itDevice:
    {
      on_actionEditDevice_triggered();
      break;
    }
      
    case itReport:
    {
      on_actionReportMain_triggered(this->model->itemFromIndex(this->currentIndex())->data(roleId).toInt());
      break;
    }
      
    case itAbsenceJournal:
    {
      on_actionAbsenceJournal_triggered();
      break;
    }
      
    case itErrandJournal:
    {
      on_actionErrandJournal_triggered();
      break;
    }
      
    case itBusinessCalendar:
    {
      on_actionBusinessCalendar_triggered();
      break;
    }
      
    case itTimeSheet:
    {
      on_actionTimeSheet_triggered();
      break;
    }
  }
  
  QTreeView::mouseDoubleClickEvent(event);
}

void SvTreeView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  actionEditDevice->setEnabled(false);
  actionDeleteDevice->setEnabled(false);
  
  actionEditScudKey->setEnabled(false);
  actionDeleteScudKey->setEnabled(false);

  actionEditEmployee->setEnabled(false);
  actionDeleteEmployee->setEnabled(false);
  
  actionEditDepartment->setEnabled(false);
  actionDeleteDepartment->setEnabled(false);
  
  QStandardItem* item = model->itemFromIndex(current);
  
  int itemType = item->type();

  switch (itemType)
  {
    case itDevice:
      actionEditDevice->setEnabled(true);
      actionDeleteDevice->setEnabled(true);
      break;
      
    case itScudKey:
    {
      actionEditScudKey->setEnabled(true);
      actionDeleteScudKey->setEnabled(true);
      break;
    }
      
    case itDepartment:
      actionEditDepartment->setEnabled(true);
      actionDeleteDepartment->setEnabled(true);
      break;
      
    case itEmployee:
      actionEditEmployee->setEnabled(true);
      actionDeleteEmployee->setEnabled(true);
      break;
      
    default:
      return;
  }

  // action`ы которые активны всегда  
  /* устройства */
  actionNewDevice->setEnabled(true);

  /* ключи скуд */
  actionNewScudKey->setEnabled(true);
  actionNewDepartment->setEnabled(true);
  actionNewEmployee->setEnabled(true);


  QTreeView::currentChanged(current, previous);
}

void SvTreeView::mousePressEvent(QMouseEvent *event)
{

  QTreeView::mousePressEvent(event);
}

/** ********** устройства *************** **/
void SvTreeView::on_actionNewDevice_triggered()
{
  DEVICE_UI = new SvDevice(this);
  if(DEVICE_UI->exec() == QDialog::Accepted)
  {
    loadDevices(true);
  }
  DEVICE_UI->~SvDevice();
}

void SvTreeView::on_actionEditDevice_triggered()
{
  DEVICE_UI = new SvDevice(this, model->itemFromIndex(this->currentIndex())->data(roleId).toInt());
  if(DEVICE_UI->exec() == QDialog::Accepted)
  {
    loadDevices(true);
  }
  DEVICE_UI->~SvDevice();
}

void SvTreeView::on_actionDeleteDevice_triggered()
{
  
}

void SvTreeView::on_actionReloadDevices_triggered()
{
  
}

/** *************** СКУД ****************** **/
void SvTreeView::on_actionScudKeys_triggered()
{
  
}

void SvTreeView::on_actionNewScudKey_triggered()
{

}

void SvTreeView::on_actionEditScudKey_triggered()
{

}

void SvTreeView::on_actionDeleteScudKey_triggered()
{

}

void SvTreeView::on_actionReloadScudKeys_triggered()
{
  
}


/** *********** ОТДЕЛЫ ****************** **/
void SvTreeView::on_actionNewDepartment_triggered()
{
  DEPARTMENT_UI = new SvDepartment(this);
  if(DEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    loadEmployees(true);
  }
  DEPARTMENT_UI->~SvDepartment();
}

void SvTreeView::on_actionEditDepartment_triggered()
{
  DEPARTMENT_UI = new SvDepartment(this, model->itemFromIndex(this->currentIndex())->data(roleId).toInt());
  if(DEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    loadEmployees(true);
  }
  DEPARTMENT_UI->~SvDepartment();
}

void SvTreeView::on_actionDeleteDepartment_triggered()
{

}

/** *********** СОТРУДНИКИ ****************** **/
void SvTreeView::on_actionNewEmployee_triggered()
{
  EMPLOYEE_UI = new SvEmployee(this);
  if(EMPLOYEE_UI->exec() == QDialog::Accepted)
  {
    loadEmployees(true);
  }
  EMPLOYEE_UI->~SvEmployee();
}

void SvTreeView::on_actionEditEmployee_triggered()
{
  EMPLOYEE_UI = new SvEmployee(this, model->itemFromIndex(this->currentIndex())->data(roleId).toInt());
  if(EMPLOYEE_UI->exec() == QDialog::Accepted)
  {
    loadEmployees(true);
  }
  EMPLOYEE_UI->~SvEmployee();
}

void SvTreeView::on_actionDeleteEmployee_triggered()
{
  
}

void SvTreeView::on_actionReloadEmployees_triggered()
{
  loadEmployees(true);
}

/** *********** ОТЧЕТЫ ****************** **/
void SvTreeView::on_actionReportMain_triggered(int reportType)
{
  REPORTSETTINGS_UI = new SvReportSettingsDialog(this, reportType == -1, true, true, false, reportType);
  if(REPORTSETTINGS_UI->exec() == QDialog::Accepted)
  {
//    qDebug() << REPORTSETTINGS_UI->t_employee_id 
//             << REPORTSETTINGS_UI->t_employee_name 
//             << REPORTSETTINGS_UI->t_period_begin.toString("dd/MM/yyyy hh:mm")
//             << REPORTSETTINGS_UI->t_period_end.toString("dd/MM/yyyy hh:mm");
    
    SUBWINDOW_MAIN_UI = new SvSubWindowMain(this->mdiWindow,
                                            REPORTSETTINGS_UI->t_department_id,
                                            REPORTSETTINGS_UI->t_employee_id,
                                            REPORTSETTINGS_UI->t_period_begin,
                                            REPORTSETTINGS_UI->t_period_end,
                                            REPORTSETTINGS_UI->t_report_type);
    SUBWINDOW_MAIN_UI->show();
  }
  REPORTSETTINGS_UI->~SvReportSettingsDialog();  
}

/** ****** ПРОИЗВОДСТВ. КАЛЕНДАРЬ *********** **/
void SvTreeView::on_actionBusinessCalendar_triggered()
{
  if(!mdiWindow) return;
  
  if(!mdiWindow->findChild<QWidget*>(SUBWINDOW_CALENDAR_NAME))
  {
    SUBWINDOW_CALENDAR = new SvSubWindowCalendar(mdiWindow);
    SUBWINDOW_CALENDAR->show();
  }
  
  SUBWINDOW_CALENDAR->setFocus();
  
}

/** ********** ЖУРНАЛ ОТСУТСТВИЙ *********** **/
void SvTreeView::on_actionAbsenceJournal_triggered()
{
  REPORTSETTINGS_UI = new SvReportSettingsDialog(this, false);
  if(REPORTSETTINGS_UI->exec() == QDialog::Accepted)
  {
    SUBWINDOW_ABSENCE_JOURNAL_UI = new SvSubWindowAbsenceJournal(mdiWindow,
                                                                 REPORTSETTINGS_UI->t_department_id,
                                                                 REPORTSETTINGS_UI->t_employee_id,
                                                                 REPORTSETTINGS_UI->t_period_begin,
                                                                 REPORTSETTINGS_UI->t_period_end);
    SUBWINDOW_ABSENCE_JOURNAL_UI->show();
  }
}

/** ********** ЖУРНАЛ МЕСТНЫХ КОМАНДИРОВОК *********** **/
void SvTreeView::on_actionErrandJournal_triggered()
{
  REPORTSETTINGS_UI = new SvReportSettingsDialog(this, false);
  if(REPORTSETTINGS_UI->exec() == QDialog::Accepted)
  {
    SUBWINDOW_ERRAND_JOURNAL_UI = new SvSubWindowErrandJournal(mdiWindow,
                                                                 REPORTSETTINGS_UI->t_department_id,
                                                                 REPORTSETTINGS_UI->t_employee_id,
                                                                 REPORTSETTINGS_UI->t_period_begin,
                                                                 REPORTSETTINGS_UI->t_period_end);
    SUBWINDOW_ERRAND_JOURNAL_UI->show();
  }
}

/** ********** ТАБЕЛЬ УЧЕТА РАБОЧЕГО ВРЕМЕНИ *********** **/
void SvTreeView::on_actionTimeSheet_triggered()
{
  REPORTSETTINGS_UI = new SvReportSettingsDialog(this, false, true, false, true);
  if(REPORTSETTINGS_UI->exec() == QDialog::Accepted)
  {
    SUBWINDOW_TIMESHEET_UI = new SvSubWindowTimeSheet(this->mdiWindow,
                                            REPORTSETTINGS_UI->t_department_id,
                                            REPORTSETTINGS_UI->t_employee_id,
                                            REPORTSETTINGS_UI->t_month,
                                            REPORTSETTINGS_UI->t_year);
    SUBWINDOW_TIMESHEET_UI->show();
  }
  
//  SUBWINDOW_ABSENCE_JOURNAL_UI->setFocus();
  
}

/** **********************************************
 *           ЗАГРУЖАЕМ ЭЛЕМЕНТЫ ДЕРЕВА 
 ********************************************** **/

void SvTreeView::loadDevices(bool reload)
{
  if(!reload)
  {
    itemDevices =  new SvDevicesItem("Устройства");        
    itemDevices->setIcon(QIcon(":/tree/icons/cologne-icons/ico/freelance.ico"));
    itemDevices->setFont(fontBold);
    
    itemRoot->appendRow(itemDevices);
  }
  else
  {
    if(!itemDevices) return;
    while(itemDevices->rowCount()) itemDevices->removeRow(0);
  }
  
  // 1. загружаем список классов устройств
  // разбито на два этапа, чтобы небыло запросов, при обработке текущего запроса
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_CLASSES_LIST).arg(""), q).type())
  {
    q->finish();
    delete q;
    return;
  }
  
  QList<QStandardItem*> classes = QList<QStandardItem*>();
  while(q->next())
  {
    QPixmap pix = QPixmap();
    pix.loadFromData(q->value("icon").toByteArray());

    classes << new SvDeviceClassItem(QIcon(pix), q->value("class_name").toString());
    classes.last()->setData(q->value("class_id").toInt(), roleId);
    classes.last()->setFont(fontBold);
    
    this->itemDevices->appendRow(classes.last());

  }
  q->finish();

  // 2. теперь загружаем список моделей по классам устройств
  while(classes.count())
  {
    QStandardItem* parent = classes.takeAt(0);

    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEVICES_LIST)
                                           .arg(QString("where devices.class_id = %1")
                                                .arg(parent->data(roleId).toInt()))
                                           , q).type())
    {
      q->finish();
      return;
    }
    
    while(q->next())
    {
      QPixmap pix = QPixmap();
      pix.load(":/tree/icons/silk/bullet_blue.png");
    
      SvDeviceItem* item = new SvDeviceItem(QIcon(pix),
                                            q->value("device_name").toString());
      
      item->setData(q->value("device_id").toInt(), roleId);
      parent->appendRow(item);

    }
    q->finish();
  }

  delete q;
  
  this->expandToDepth(0);
  
}

void SvTreeView::loadEmployees(bool reload)
{
  if(!reload)
  {
    itemEmployees =  new SvEmployeesItem("Сотрудники");        
    itemEmployees->setIcon(QIcon(":/tree/icons/cologne-icons/ico/customers.ico"));
    itemEmployees->setFont(fontBold);
    
    itemRoot->appendRow(itemEmployees);
  }
  else
  {
    if(!itemEmployees) return;
    while(itemEmployees->rowCount()) itemEmployees->removeRow(0);
  }
  
  // 1. загружаем список отделов
  // разбито на два этапа, чтобы небыло запросов, при обработке текущего запроса
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEPARTMENTS_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  QList<SvDepartmentItem*> departments = QList<SvDepartmentItem*>();
  while(q->next())
  {
    QPixmap pix = QPixmap();
    pix.loadFromData(q->value("icon").toByteArray());
//    pix.load(":/tree/icons/cologne-icons/ico/category.ico");
    
    departments << new SvDepartmentItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)), q->value("department_name").toString());
    departments.last()->setData(q->value("department_id").toInt(), roleId);
    departments.last()->setFont(fontBold);
    
    this->itemEmployees->appendRow(departments.last());

  }
  q->finish();
  
  // 2. теперь загружаем список сотрудников по отделам
  while(departments.count())
  {
    SvDepartmentItem* parent = departments.takeAt(0);

    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEES_LIST_LIGHT)
                                           .arg(QString("where employees.department_id = %1 and deleted = false")
                                                .arg(parent->data(roleId).toInt()))
                                           , q).type())
    {
      q->finish();
      return;
    }
    
    while(q->next())
    {
      QPixmap pix = QPixmap();
      pix.load(":/tree/icons/cologne-icons/ico/administrative-docs.ico");
    
      SvEmployeeItem* item = new SvEmployeeItem(QIcon(pix),
                                                QString("%1 (%2)")
                                                      .arg(q->value("short_name").toString())
                                                      .arg(q->value("employee_position").toString()));

      item->setData(q->value("employee_id"), roleId);
      parent->appendRow(item);

    }
    q->finish();
  }

  // 3. теперь загружаем список сотрудников, не относящихся ни к одному из отделов
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEES_LIST_LIGHT)
                                         .arg("where employees.department_id = -1 and deleted = false"), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    QPixmap pix = QPixmap();
    pix.load(":/tree/icons/cologne-icons/ico/administrative-docs.ico");
  
    SvEmployeeItem* item = new SvEmployeeItem(QIcon(pix),
                                              QString("%1 (%2)")
                                                    .arg(q->value("short_name").toString())
                                                    .arg(q->value("employee_position").toString()));
    
    item->setData(q->value("employee_id"), roleId);
    itemEmployees->appendRow(item);

  }
  q->finish();
  
  
  delete q;
  
  this->expandToDepth(0);
}

void SvTreeView::loadReports(bool reload)
{
  if(!reload)
  {
    itemReports =  new SvReportsItem(actionReportMain->text());        
    itemReports->setIcon(QIcon(":/tree/icons/cologne-icons/ico/old-versions.ico"));
    itemReports->setFont(fontBold);
    
    itemRoot->appendRow(itemReports);
  }
  else
  {
    if(!itemReports) return;
    while(itemReports->rowCount()) itemReports->removeRow(0);
  }
  
  // 1. загружаем список отчетов
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_REPORT_TYPES).arg(""), q).type())
  {
    q->finish();
    delete q;
    return;
  }
  
  QList<SvReportItem*> reports = QList<SvReportItem*>();
  while(q->next())
  {
    QPixmap pix = QPixmap();
    pix.load(":/tree/icons/cologne-icons/ico/current-work.ico");
    
    reports << new SvReportItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)), q->value("report_type_name").toString());
    reports.last()->setData(q->value("report_type_id").toInt(), roleId);
//    reports.last()->setFont(fontBold);
    
    this->itemReports->appendRow(reports.last());

  }
  q->finish();
  delete q;
  
  this->expandToDepth(0);
}

void SvTreeView::loadJournals(bool reload)
{
  if(!reload)
  {
    itemTimeAccounting =  new SvTimeAccountingItem("Учет рабочего времени");        
    itemTimeAccounting->setIcon(QIcon(":/tree/icons/cologne-icons/ico/pen.ico"));
    itemTimeAccounting->setFont(fontBold);
    
    itemRoot->appendRow(itemTimeAccounting);
  }
  else
  {
    if(!itemTimeAccounting) return;
    while(itemTimeAccounting->rowCount()) itemTimeAccounting->removeRow(0);
  }
  
  /* СНАЧАЛА ДОЛЖНЫ БЫТЬ СОЗДАНЫ ACTION'ы */  
  itemAbsenceJournal = new SvAbsenceJournalItem(actionAbsenceJournal->icon(), actionAbsenceJournal->text());
  itemErrandJournal = new SvErrandJournalItem(actionErrandJournal->icon(), actionErrandJournal->text());
  itemBusinessCalendar = new SvBusinessCalendarItem(actionBusinessCalendar->icon(), actionBusinessCalendar->text());
  itemTimeSheetItem = new SvTimeSheetItem(actionTimeSheet->icon(), actionTimeSheet->text());
  
  itemTimeAccounting->appendRow(itemAbsenceJournal);
  itemTimeAccounting->appendRow(itemErrandJournal);
  itemTimeAccounting->appendRow(itemBusinessCalendar);
  itemTimeAccounting->appendRow(itemTimeSheetItem);
  
  this->expandToDepth(0);
}
    
    
    
    
    
    
    
