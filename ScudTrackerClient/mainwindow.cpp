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
  
  ip = sett->readValue("SERVER", "ip", "").toString();
  port = sett->readValue("SERVER", "port", 35550).toInt();
  
  sett->~SvSettings();
  
//  tcp_server = new SvTcpServer(35580, false, this, false, false, false, false, debug);

  connect(ui->bnStart, SIGNAL(pressed()), this, SLOT(slotStart()));
  connect(ui->bnStop, SIGNAL(pressed()), this,  SLOT(slotStop()));
  connect(ui->bnSettings, SIGNAL(pressed()), this,  SLOT(slotSettings()));
//  connect(tcp_server, SIGNAL(sigClientDisconnected()), this, SLOT(slotGotMessage()));  
  
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
  sett->~SvSettings();
  
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
  if(!QHostAddress(ip).toIPv4Address())
  {
    QMessageBox::critical(this, "Ошибка", "В ini файле IP адрес не задан, либо задан неверно", QMessageBox::Ok);
    return;
  }
  
  ui->bnStart->setEnabled(false);
  ui->bnStop->setEnabled(true);
  
//  tcp_server->startServer(35580);
  
  /**************************/
  ConnectToRemoteHost *host = new ConnectToRemoteHost(0, ip, port, 1000);
  workerThread = new QThread(this);
  QObject::connect(workerThread, &QThread::started, host, &ConnectToRemoteHost::getHostStatus);
  QObject::connect(workerThread, &QThread::finished, host, &ConnectToRemoteHost::deleteLater);
  QObject::connect(host, SIGNAL(status(bool)), this, SLOT(drawStatus(bool)));
  QObject::connect(host, SIGNAL(lastData(QString)), this, SLOT(drawLastData(QString)));
//  QObject::connect(host, SIGNAL(status(bool)), this, SLOT(slotGotMessage());
  
  host->moveToThread(workerThread);
  workerThread->start();
  
}

void MainWindow::slotStop()
{
//  tcp_server->stopServer();
  
  workerThread->quit();
  while(workerThread->isRunning()) qApp->processEvents();
  
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

void MainWindow::drawLastData(QString lastData)
{
  ui->lblLast->setText(lastData);
}

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
  connect(socket, SIGNAL(readyRead()), this, SLOT(getKeys()));
  
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(getHostStatus()));
  timer->start(timeOut);
  
  awaitResponse = new QTimer(this);
  connect(awaitResponse, SIGNAL(timeout()), this, SLOT(disconnectFormHost()));
  awaitResponse->start(timeOut);
  
}

ConnectToRemoteHost::~ConnectToRemoteHost()
{
  timer->stop();
  awaitResponse->stop();
  this->deleteLater();
}

void ConnectToRemoteHost::getHostStatus()
{
  
  online = false;
  
  socket->connectToHost(ip, port);
  if(socket->waitForConnected(100))
  {
    timer->stop();
    online = true;
    
    QByteArray b = QByteArray();
    b.append("GET_KEYS");
    
    socket->write(b);

    awaitResponse->start(1000);
  }

  emit status(online);
  
}

void ConnectToRemoteHost::disconnectFormHost()
{
  socket->disconnectFromHost();
  timer->start();
}

void ConnectToRemoteHost::getKeys()
{
  awaitResponse->stop();

  if(socket->bytesAvailable() > 0)
  {
    QByteArray data = QByteArray();
    data = socket->readAll();
    
    // выделяем память под данные
    void* packet = malloc(data.size());
    memcpy(packet, data.data(), data.size());
  
    GotKeyStruct* gotKey = reinterpret_cast<GotKeyStruct*>(malloc(sizeof(GotKeyStruct)));
    memset(gotKey, char(0), sizeof(GotKeyStruct));
  
    /** читаем количество ключей в пакете **/
    quint32 key_cnt = 0;
    memcpy(&key_cnt, packet, sizeof(key_cnt));
  
    /** используем дату/время сервера, а не контроллера ! */
    QDateTime t_date_time;
    t_date_time.setDate(QDateTime::currentDateTime().date());
  
    /* делаем так, чтобы обнулить миллисекунды и отсечь повторы с контроллера */
    QTime tm = QDateTime::currentDateTime().time();
    t_date_time.setTime(QTime(tm.hour(), tm.minute(), tm.second(), 0));
    /** ********* **/
   
    /** читаем ключи из пакета и кладем их в базу **/
    for(int i = 0; i < key_cnt; i++)
    {
      memcpy(gotKey, packet + sizeof(key_cnt) + i * sizeof(GotKeyStruct), sizeof(GotKeyStruct));
  
      /* читаем какой то код и номер считывателя */
      int t_scud_number = int(gotKey->scud_number);
  
      /* какой то код */
      int t_code = t_scud_number >> 4;
  
      /* номер считывателя */
      t_scud_number = t_scud_number & 0x0f;
  
      /* сам ключ */
      int t_key = 0;
      memcpy(&t_key, &gotKey->key_byte1, 3);
  
      /* команда считывателя */
      uchar t_sensor_command = gotKey->sensor_command;
  
      /* кидаем все что приходит в общую таблицу-мусорку */
      PGDB->execSQL(QString(SQL_NEW_PASSAGE_TRASH)
                                .arg(t_key)
                                .arg(t_scud_number)
                                .arg(qint32(socket->peerAddress().toIPv4Address()))
                                .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
                                .arg(qint32(t_sensor_command) + (key_cnt << 8)), false);
  
      /* отсекаем повторяющиеся значения ключа/даты */
      if((last_key == t_key) && (last_date_time == t_date_time)) continue;

      emit lastData(QString("Key: %1  Time: %2  IP: %3  Door: %4  Code: %5  Key count: %6  code: %7")
                             .arg(t_key)
                             .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss"))
                             .arg(QString::number(socket->peerAddress().toIPv4Address(), 16))
                             .arg(t_scud_number)
                             .arg(t_code)
                             .arg(key_cnt)
                             .arg(t_sensor_command));
  
      last_key = t_key;
      last_date_time = t_date_time;

      /* отсекаем нули */
      if(!t_key) continue;
      
      /* отсекаем FFFFFF */
      if(t_key == 0xFFFFFF) continue;
  
      QSqlError err = PGDB->execSQL(QString(SQL_NEW_PASSAGE)
                                            .arg(t_key)
                                            .arg(t_scud_number)
                                            .arg(qint32(socket->peerAddress().toIPv4Address()))
                                            .arg(t_date_time.toString("dd/MM/yyyy hh:mm:ss")), false);
    }
  
    free(gotKey);
    free(packet);
    
  }
  
  this->disconnectFormHost();
  
}
