#include "mainwindow.h"

using namespace log_ns;

extern SvPGDB* PGDB;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  
  QString s = QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  this->resize(sett->readValue("MAIN_WINDOW", "Size", QSize(400, 200)).toSize());
  this->move(sett->readValue("MAIN_WINDOW", "Position", QPoint(100, 100)).toPoint());
  debug = sett->readValue("MAIN_WINDOW", "debug", false).toBool();
  
  loadDepartments();
  
  ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());  
  
  connect(ui->cbDepartment, SIGNAL(currentIndexChanged(int)), this, SLOT(loadEmployees(int)));
  connect(ui->bnSave, SIGNAL(pressed()), this, SLOT(slotSave()));
//  connect(ui->bnExit, SIGNAL(pressed()), this,  SLOT(slotStop()));
}

MainWindow::~MainWindow()
{
  PGDB->~SvPGDB();
  
  QString s = QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".ini";

  SvSettings *sett = new SvSettings(s, this);
  sett->writeValue("MAIN_WINDOW", "Size", this->size());
  sett->writeValue("MAIN_WINDOW", "Position", this->pos());
  delete ui;
}

void MainWindow::loadDepartments()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEPARTMENTS_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    QPixmap pix = QPixmap(16, 16);
    pix.loadFromData(q->value("icon").toByteArray());
    ui->cbDepartment->addItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)),
                         q->value("department_name").toString(),
                         q->value("department_id").toInt());
  }

  q->finish();
  
  if(ui->cbDepartment->count())
  {
    ui->cbDepartment->setCurrentIndex(4); // Департамент поизводства РТИ
    loadEmployees(4);
  }

}

void MainWindow::loadEmployees(int idx)
{
  int department_id = ui->cbDepartment->currentData().toInt();
  
  ui->cbEmployee->clear();
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEES_LIST_LIGHT)
                                         .arg(QString("where department_id = %1")
                                              .arg(department_id)), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    ui->cbEmployee->addItem(q->value("short_name").toString(),
                         q->value("employee_id").toInt());
  }

  q->finish();
  
  if(ui->cbEmployee->count()) ui->cbEmployee->setCurrentIndex(0);

}

QTextEdit* MainWindow::get_log()
{
  return ui->textLog;
}

void MainWindow::slotSave()
{
  qint32 ip = QHostAddress(ui->editIp->text()).toIPv4Address();
  if(!ip)
  {
    log_ns::log(m_Critical, "Неверный IP адрес устройства");
    return;
  }
  
  ui->bnSave->setEnabled(false);
  
  /** количество ключей в пакете (здесь всегда 1) **/
  quint32 key_cnt = 1;
  
  /** датa/время  */
  t_date_time.setDate(ui->dateTimeEdit->date());
  t_date_time.setTime(ui->dateTimeEdit->time());
  
  /** находим первый ключ, который относится к выбранному сотруднику **/
  QSqlQuery *q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString("select key_value from scuds "
                                                 "where employee_id = %1 "
                                                 "order by id asc limit 1")
                                         .arg(ui->cbEmployee->currentData().toInt()), q).type())
  {
    q->finish();
    delete q;
    return;
  }
  /* сам ключ */
  q->first();
  t_key = q->value("key_value").toInt();
  q->finish();

  /* номер считывателя */
  t_scud_number = ui->rbIncome->isChecked() ? 1 : 2;
  
  /* какой то код */
  t_code = 0; //t_scud_number >> 4;
  
  /* кидаем все в общую таблицу-мусорку */
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_PASSAGE_TRASH)
                              .arg(t_key)
                              .arg(t_scud_number)
                              .arg(ip)
                              .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
                              .arg(qint32(key_cnt << 8)), debug).type())
  {
    log_ns::log(m_Err, "Ошибка при добавлении записи в базу");
    return;
  }

  /* кидаем все в рабочую таблицу */
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_PASSAGE)
                                        .arg(t_key)
                                        .arg(t_scud_number)
                                        .arg(ip)
                                        .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))).type())
  {
    log_ns::log(m_Err, "Ошибка при добавлении записи в базу");
    return;
  }
  
  QFile file(QApplication::applicationDirPath() + "/dumb_history.txt");
  if (!file.open(QIODevice::Append | QIODevice::Text))
  {
    log_ns::log(m_Fail, "File dumb_history.txt not found");
    return;
  }
    
  QTextStream out(&file);
  out << QString("department:%1(%2); employee:%3(%4); key_value:%5; date_time:'%6'; scud_number:%7; device_ip:%8\n")
         .arg(ui->cbDepartment->currentText())
         .arg(ui->cbDepartment->currentData().toInt())
         .arg(ui->cbEmployee->currentText())
         .arg(ui->cbEmployee->currentData().toInt())
         .arg(t_key)
         .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
         .arg(t_scud_number)
         .arg(ui->editIp->text());
  
  file.close();
  
  QMessageBox::information(this, "Успех", "Данные записаны успешно", QMessageBox::Ok);
  ui->bnSave->setEnabled(true);
}

