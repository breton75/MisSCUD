#include "mainwindow.h"

using namespace log_ns;

extern SvPGDB* PGDB;
extern SvTrackerSettings* TRACKERSETTINGS_UI;

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
  ui->dockWidget_2->resize(sett->readValue("DOCK_WIDGET", "Size", QSize(400, 400)).toSize());
  ui->dockWidget_2->move(sett->readValue("DOCK_WIDGET", "Position", QPoint(300, 300)).toPoint());
  
  tcp_server = new SvTcpServer(35580, false, this, false, false, false, false, debug);

  connect(ui->bnStart, SIGNAL(pressed()), this, SLOT(slotStart()));
  connect(ui->bnStop, SIGNAL(pressed()), this,  SLOT(slotStop()));
  connect(ui->bnSettings, SIGNAL(pressed()), this,  SLOT(slotSettings()));
  connect(tcp_server, SIGNAL(sigClientDisconnected()), this, SLOT(slotGotMessage()));  
  
}

MainWindow::~MainWindow()
{
  PGDB->~SvPGDB();
  
  QString s = QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".ini";

  SvSettings *sett = new SvSettings(s, this);
  sett->writeValue("MAIN_WINDOW", "Size", this->size());
  sett->writeValue("MAIN_WINDOW", "Position", this->pos());
  sett->writeValue("MAIN_WINDOW", "debug", debug);
  sett->writeValue("DOCK_WIDGET", "Size", ui->dockWidget_2->size());
  sett->writeValue("DOCK_WIDGET", "Position", ui->dockWidget_2->pos());
  delete ui;
}

void MainWindow::slotClose()
{

}

QTextEdit* MainWindow::get_log()
{
  return ui->textLog;
}

void MainWindow::slotStart()
{
  ui->bnStart->setEnabled(false);
  ui->bnStop->setEnabled(true);
  
  tcp_server->startServer(35580);
  
  /**************************/
  ConnectToRemoteHost *host = new ConnectToRemoteHost(0, "192.168.3.36", 35550, 1000);
  workerThread = new QThread(this);
  QObject::connect(workerThread, &QThread::started, host, &ConnectToRemoteHost::getHostStatus);
  QObject::connect(workerThread, &QThread::finished, host, &ConnectToRemoteHost::deleteLater);
  QObject::connect(host, SIGNAL(status(bool)), this, SLOT(drawStatus(bool)));
  
  host->moveToThread(workerThread);
  workerThread->start();
  
}

void MainWindow::slotStop()
{
  tcp_server->stopServer();
  
  workerThread->quit();
  while(workerThread->isRunning()) qApp->processEvents();
//  free(workerThread);
  
  ui->bnStart->setEnabled(true);
  ui->bnStop->setEnabled(false);
}

void MainWindow::slotSettings()
{
  TRACKERSETTINGS_UI = new SvTrackerSettings(this);
  if(TRACKERSETTINGS_UI->exec() == QDialog::Accepted)
  {
    
  }
  TRACKERSETTINGS_UI->~SvTrackerSettings();
}

void MainWindow::slotGotMessage()
{
  // выделяем память под данные
  GotKeyStruct* gotKey = reinterpret_cast<GotKeyStruct*>(malloc(sizeof(GotKeyStruct)));
  memset(gotKey, char(0), sizeof(GotKeyStruct));
  
  /** читаем количество ключей в пакете **/
  quint32 key_cnt = 0;
  memcpy(&key_cnt, tcp_server->last_message, sizeof(quint32));
  
  /** используем дату/время сервера, а не контроллера ! */
  t_date_time.setDate(QDateTime::currentDateTime().date());
  
  /* делаем так, чтобы обнулить миллисекунды и отсечь повторы с контроллера */
  QTime tm = QDateTime::currentDateTime().time();
  t_date_time.setTime(QTime(tm.hour(), tm.minute(), tm.second(), 0));
  /** ********* **/
  
  /** читаем ключи из пакета и кладем их в базу **/
  for(int i = 0; i < key_cnt; i++)
  {
    memcpy(gotKey, tcp_server->last_message + 1 + i * sizeof(GotKeyStruct), sizeof(GotKeyStruct));
  
    /* читаем какой то код и номер считывателя */
    t_scud_number = int(gotKey->scud_number);
    
    /* какой то код */
    t_code = t_scud_number >> 4;
    
    /* номер считывателя */
    t_scud_number = t_scud_number & 0x0f;
    
    /* сам ключ */
    t_key = 0;
    memcpy(&t_key, &gotKey->key_byte1, 3);
    
//    /* дата и время контроллера */
//    QDate d = QDate((gotKey->year  >> 4) * 10 + (gotKey->year  & 0x0F),
//                    (gotKey->month >> 4) * 10 + (gotKey->month & 0x0F),
//                    (gotKey->day   >> 4) * 10 + (gotKey->day   & 0x0F));
    
//    QTime t = QTime((gotKey->hour >> 4) * 10 + (gotKey->hour & 0x0F),
//                    (gotKey->min  >> 4) * 10 + (gotKey->min  & 0x0F),
//                    (gotKey->sec  >> 4) * 10 + (gotKey->sec  & 0x0F));
  
//    t_date_time.setDate(d);
//    t_date_time.setTime(t);

    
    /* кидаем все что приходит в общую таблицу-мусорку */
    //  if(!t_key || ((last_key == t_key) && (last_date_time == t_date_time)))
    //  {
        PGDB->execSQL(QString(SQL_NEW_PASSAGE_TRASH)
                              .arg(t_key)
                              .arg(t_scud_number)
                              .arg(qint32(QHostAddress(tcp_server->lastClientIp).toIPv4Address()))
                              .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss")), debug);
    //    return;
    //  }
    
      /* отсекаем повторяющиеся значения ключа/даты */
      if((last_key == t_key) && (last_date_time == t_date_time)) continue;
    
      ui->lblLast->setText(QString("Key: %1  Time: %2  IP: %3  Door: %4  Code: %5  Key count: %6")
                           .arg(t_key)
                           .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
                           .arg(QString::number(QHostAddress(tcp_server->lastClientIp).toIPv4Address(), 16))
                           .arg(t_scud_number)
                           .arg(t_code)
                           .arg(key_cnt));
    
      last_key = t_key;
      last_date_time = t_date_time;
    
    
    
      /* отсекаем нули */
      if(!t_key) continue;
      
      QSqlError err = PGDB->execSQL(QString(SQL_NEW_PASSAGE)
                                            .arg(t_key)
                                            .arg(t_scud_number)
                                            .arg(qint32(QHostAddress(tcp_server->lastClientIp).toIPv4Address()))
                                            .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss")), debug);
    
      if(err.type() != QSqlError::NoError)
      {
        log_ns::log(m_Err, err.databaseText());
        return;
      }
  }
  
  free(gotKey);
}

/** один ключ в пакете **/
//void MainWindow::slotGotMessage()
//{
//  GotKeyStruct* gotKey = reinterpret_cast<GotKeyStruct*>(malloc(sizeof(GotKeyStruct)));
//  memset(gotKey, char(0), sizeof(GotKeyStruct));
//  memcpy(gotKey, tcp_server->last_message, sizeof(GotKeyStruct));
  
//  /* номер считывателя */
//  t_scud_number = int(gotKey->scud_number);
//  t_code = t_scud_number >> 4;
//  t_scud_number = t_scud_number & 0x0f;
  
//  /* сам ключ */
//  t_key = 0;
//  memcpy(&t_key, &gotKey->key_byte1, 3);
  
//  /* дата и время */
//  QDate d = QDate((gotKey->year  >> 4) * 10 + (gotKey->year  & 0x0F),
//                  (gotKey->month >> 4) * 10 + (gotKey->month & 0x0F),
//                  (gotKey->day   >> 4) * 10 + (gotKey->day   & 0x0F));
  
//  QTime t = QTime((gotKey->hour >> 4) * 10 + (gotKey->hour & 0x0F),
//                  (gotKey->min  >> 4) * 10 + (gotKey->min  & 0x0F),
//                  (gotKey->sec  >> 4) * 10 + (gotKey->sec  & 0x0F));

////  QDateTime t_date_time;
//  t_date_time.setDate(d);
//  t_date_time.setTime(t);
  
//  /** временно ! */
//  t_date_time.setDate(QDateTime::currentDateTime().date());
  
//  /* делаем так, чтобы обнулить миллисекунды. иначе идут повторы с контроллера */
//  QTime tm = QDateTime::currentDateTime().time();
//  t_date_time.setTime(QTime(tm.hour(), tm.minute(), tm.second(), 0));
//  /** ********* **/
  
//  /* день недели */
////  int t_weekday = gotKey->weekday;
  
//  free(gotKey);
  
//  /* отсекаем нули и повторяющиеся значения ключа/даты. кидаем их в мусорку */
//  if(!t_key || ((last_key == t_key) && (last_date_time == t_date_time)))
//  {
//    PGDB->execSQL(QString(SQL_NEW_PASSAGE_TRASH)
//                          .arg(t_key)
//                          .arg(t_scud_number)
//                          .arg(QHostAddress(tcp_server->lastClientIp).toIPv4Address())
//                          .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss")), debug);
//    return;
//  }
  
//  ui->lblLast->setText(QString("Key: %1  Time: %2  IP: %3  Door: %4  Code: %5")
//                       .arg(t_key)
//                       .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
//                       .arg(QString::number(QHostAddress(tcp_server->lastClientIp).toIPv4Address(), 16))
//                       .arg(t_scud_number)
//                       .arg(t_code));
  
//  last_key = t_key;
//  last_date_time = t_date_time;
  
//  /* отсекаем нули */
//  //if(!t_key) return;
  
//  QSqlError err = PGDB->execSQL(QString(SQL_NEW_PASSAGE)
//                                        .arg(t_key)
//                                        .arg(t_scud_number)
//                                        .arg(qint32(QHostAddress(tcp_server->lastClientIp).toIPv4Address()))
//                                        .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss")), debug);

//  if(err.type() != QSqlError::NoError)
//  {
//    log_ns::log(m_Err, err.databaseText());
//    return;
//  }
  
  
////  QSqlQuery* q = new QSqlQuery(PGDB->db);
////  PGDB->execSQL(QString(SQL_SELECT_SCUDKEYS_LIST).arg(QString("where key_value = ").arg(t_key)), q, debug);
////  q->first();
  
////  QSqlQuery* q1 = new QSqlQuery(PGDB->db);
////  PGDB->execSQL(QString("select device_name from devices where ip = '%1'").arg(tcp_server->lastClientIp), q1, debug);
////  q1->first();

////  log_ns::log(m_Data, QString("Ключ: %1  Сотрудник: <%2 %3. %4.>   Время прохода: %5  Дверь: %6  Считыватель: %7  Код: %8")
////                .arg(t_key)
////                .arg(q->value("employee_surename").toString())
////                .arg(q->value("employee_name").toString() == "" ? '\0' : q->value("employee_name").toString().at(0))
////                .arg(q->value("employee_patronymic").toString() == "" ? '\0' : q->value("employee_patronymic").toString().at(0))     
////                .arg(t_date_time.toString("dd.MM.yy hh:mm:ss"))
////                .arg(QString("%1 (%2)").arg(q1->value("device_name").toString()).arg(tcp_server->lastClientIp))
////                .arg(t_scud_number)
////                .arg(t_code));
    
////  q->~QSqlQuery();
////  q1->~QSqlQuery();
////  free(q);
////  free(q1);
////  pg->~SvPGDB();
  
//}

void MainWindow::drawStatus(bool online)
{
  QPixmap pix = QPixmap(16, 16);
  ui->lblStatusImg->setPixmap(pix);

  switch (online)
  {
    case true:
      if(imgCounter)pix.load(":/device_config/icons/cologne-icons/ico/communication.ico");
      else pix.load(":/device_config/icons/cologne-icons/ico/communication_rev.ico");
      if(!lastStatus)
        log(m_Success, "Соединение установлено");
      break;
      
    default:
      if(imgCounter)pix.load(":/employee/icons/silk/cross.ico");
      else pix.load(":/employee/icons/silk/cross_rev.ico");
      if(lastStatus)
        log(m_Fail, "Соединение потеряно");
      break;
  }

  ui->lblStatusImg->setPixmap(pix);
  imgCounter = !imgCounter;
  lastStatus = online;
  
}


void MainWindow::on_bnTest_clicked()
{
//  PGDB->execSQL("EXEC PROCEDURE")
}


ConnectToRemoteHost::ConnectToRemoteHost(QObject *parent, QString ip, quint16 port, int timeOut) :
    QObject(parent)
{
  this->ip = ip;
  this->port = port;
  
  socket = new QTcpSocket(this);
  timer = new QTimer(this);

  connect(timer, SIGNAL(timeout()), this, SLOT(getHostStatus()));
  timer->start(timeOut);

}

ConnectToRemoteHost::~ConnectToRemoteHost()
{
  this->deleteLater();
}

void ConnectToRemoteHost::getHostStatus()
{
  online = false;
  
  socket->connectToHost(ip, port);
  if(socket->waitForConnected(100))
  {
    online = true;
    
    socket->disconnectFromHost();
  }

  emit status(online);
  
}
