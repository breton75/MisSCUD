#include "sv_device_config.h"

SvDeviceConfiguration* DEVICE_CONFIG_UI;
extern SvPGDB* PGDB;

using namespace log_ns;

SvDeviceConfiguration::SvDeviceConfiguration(int deviceId)
{
  this->setWindowTitle("Конфигуратор СКУД");
  
  setupUi();
  
  cbSensorType->addItem("Wiegand 26", 0);
  cbSensorType->addItem("iButton (эмуляция DS1990A)", 1);
  cbSensorType->setCurrentIndex(1);  
  
  this->device_id = deviceId;
  
//  if(deviceId != -1)
//  {
//    QSqlQuery* q = new QSqlQuery(PGDB->db);
//    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEVICE).arg(deviceId), q).type())
//    {
//      q->finish();
//      return;
//    }
//    q->first();
    
//    t_device_name = q->value("device_name").toString();
    
//    if(!q->value("device_config").toByteArray().isNull())
//    {
//      DeviceSettings as = parseConfig(q->value("device_config").toByteArray());
      
//  //    t_device_current_ip = as.device_current_ip;
//  //    t_device_current_port = as.device_current_port;
//  //    t_device_new_ip = as.device_new_ip;
//  //    t_device_new_port = as.device_new_port;
//      t_server_ip = as.server_ip;
//      t_server_port = as.server_port;
//    }
//    q->finish();
//  }
  
//  this->editDeviceCurrentIP->setText(t_device_current_ip);
//  this->editDeviceCurrentPort->setText(QString::number(t_device_current_port));
//  this->editDeviceNewIP->setText(t_device_new_ip);
//  this->editDeviceNewPort->setText(QString::number(t_device_new_port));
  
  connect(bnApply, SIGNAL(clicked()), this, SLOT(apply()));
  connect(bnRequest, SIGNAL(clicked()), this, SLOT(request()));
  connect(bnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  
//  QMetaObject::connectSlotsByName(Dialog);
  
  this->setModal(true);
  this->show();
  
}

SvDeviceConfiguration::~SvDeviceConfiguration()
{
  this->close();
  this->deleteLater();
}

void SvDeviceConfiguration::setCurrentData(QString ip, quint16 port)
{
  t_device_current_ip = (QHostAddress(ip).toIPv4Address() == 0 ? "192.168.3.31" : ip);
  t_device_current_port = (port == 0 ? 35550 : port);
  
  this->editDeviceCurrentIP->setText(t_device_current_ip);
  this->editDeviceCurrentPort->setText(QString::number(t_device_current_port));
  
  this->editDeviceNewIP->setText(t_device_current_ip);
  this->editDeviceNewPort->setText(QString::number(t_device_current_port));
}

bool SvDeviceConfiguration::checkInput(bool onlyCurrent)
{
  if(QHostAddress(editDeviceCurrentIP->text()).toIPv4Address() == 0) {
    log_ns::log(m_Critical, "Неверно указан текущий IP адрес");
    editDeviceCurrentIP->setFocus();
    return false;   }
  
  if(QString(editDeviceCurrentPort->text()).toInt() == 0) {
    log_ns::log(m_Critical, "Неверно указан текущий порт");
    editDeviceCurrentPort->setFocus();
    return false;   }
  
  if(onlyCurrent) return true;
  
  if(QHostAddress(editDeviceNewIP->text()).toIPv4Address() == 0){
    log_ns::log(m_Critical, "Неверно задан новый IP адрес устройства", this);
    editDeviceNewIP->setFocus();
    return false;  }
  
  if(editDeviceNewPort->text().toInt() == 0){
    log_ns::log(m_Critical, "Неверно задан новый порт устройства", this);
    editDeviceNewPort->setFocus();
    return false;  }
  
  if(QHostAddress(editServerIP->text()).toIPv4Address() == 0)  {
    log_ns::log(m_Critical, "Неверно задан новый IP адрес сервера", this);
    editServerIP->setFocus();
    return false;   }  
  
  if(editServerPort->text().toInt() == 0)  {
    log_ns::log(m_Critical, "Неверно задан новый порт сервера", this);
    editServerPort->setFocus();
    return false;   } 
  
  return true;
}

void SvDeviceConfiguration::apply()
{
  /** проверяем правильность введенных данных **/
  if(!checkInput()) return;

  setEnabledWidgets(false);
  
  /** формируем данные конфигурации для отправки на устройство **/
  quint32 new_device_ip = qToBigEndian(QHostAddress(editDeviceNewIP->text()).toIPv4Address());
  quint32 new_device_port = quint32(editDeviceNewPort->text().toInt());
  
  quint32 server_ip = qToBigEndian(QHostAddress(editServerIP->text()).toIPv4Address());
  quint32 server_port = quint32(editServerPort->text().toInt());

  IP_my_srv config = IP_my_srv();
  
  /* GET_SYS_CONFIG */
  const char cmd[] = {0x53, 0x45, 0x54, 0x5F, 0x53, 0x59, 0x53, 0x5F, 0x43, 0x4F, 0x4E, 0x46, 0x49, 0x47};
  
  /* новый адрес устройства */
  memcpy(&config.IP[0], &new_device_ip, sizeof(new_device_ip));
  
  /* новый порт устройства */
  config.Port = new_device_port;
  
  /* новый МАС устройства */
  config.MAC[0] = 0xC0;
  config.MAC[1] = 0x4D;
  config.MAC[2] = 0x49;
  config.MAC[3] = 0x53;
  config.MAC[4] = 0x00;
  config.MAC[5] = config.IP[3];
  
  /* количество серверов для отправки данных */
  //config.kol = 1;
  
  /* тип подключенных считывателей */
  config.sensorType = cbSensorType->currentData().toInt();
  
  /* новый адрес сервера для отправки данных */
  memcpy(&config.ip_serv.IP[0], &server_ip, sizeof(server_ip)); 
  
  /* новый МАС сервера для отправки данных (равен 0) */
  memset(&config.ip_serv.MAC[0], 0, 6);
  
  /* новый порт сервера для отправки данных */
  config.ip_serv.Port = server_port;
  

  /** отправляем новую конфигурацию **/
  
  /* формируем команду */
  QByteArray b = QByteArray();
  b.append((const char*)(&cmd), sizeof(cmd));
  b.append((const char*)(&config), sizeof(config));
  
  /* отправляем данные */
  bool result = false;
  SvTcpClient* client = new SvTcpClient(this->editDeviceCurrentIP->text(), this->editDeviceCurrentPort->text().toInt(), this, true, true, true, true);
  
  switch (client->connectToHost())
  {
    case SOCKET_OK:
    {
      client->sendData(&b, 2000, this);
      
      switch (client->responseStatus)
      {
        case SOCKET_OK:
        {
          QString s = QString(client->responseData);
          result = s == "OK";
          client->disconnectFromHost();
          break;
        }
          
        case SOCKET_AWAITING_CANCELED:
          log_ns::log(m_Critical, "Отменено пользователем");
          break;
          
        case SOCKET_TIMEOUT:
          log_ns::log(m_Critical, "Время ожидания ответа истекло");
          break;
          
        default:
          client->socket->abort();
          log_ns::log(m_Critical, "Операция не завершена");
          break;
      }
      
      break;
    }
      
    default:
    {
      log_ns::log(m_Critical, "Не удалось подключиться к устройству");
      break;
    }
  }
      
  delete client;
  
  if(result)
  {
    t_device_new_ip = this->editDeviceNewIP->text();
    t_device_new_port = this->editDeviceNewPort->text().toInt();
    t_server_ip = this->editServerIP->text();
    t_server_port = this->editServerPort->text().toInt();
    
    QMessageBox::information(this, "Успешно", "Конфигурация была успешно применена", QMessageBox::Ok);
        
    QDialog::accept();
  }
  
  setEnabledWidgets(true);
  
}

void SvDeviceConfiguration::accept()
{
  
}

void SvDeviceConfiguration::request()
{
  /** проверяем правильность введенных данных **/
  if(!checkInput(true)) return;
  
  setEnabledWidgets(false);

  // запрашиваем текущую конфигурацию
  SvTcpClient* client = new SvTcpClient(editDeviceCurrentIP->text(), quint16(QString(editDeviceCurrentPort->text()).toInt()));
  
  switch (client->connectToHost())
  {
    case SOCKET_OK:
    {
      IP_my_srv* m;
      
      client->sendData("GET_SYS_CONFIG", 5000, this);
      
      switch (client->responseStatus)
      {
        case SOCKET_OK:
        {
          m = (IP_my_srv*)(malloc(sizeof(IP_my_srv)));
          memcpy(m, client->responseData.data(), sizeof(IP_my_srv));
          
          this->editServerIP->setText(QString("%1.%2.%3.%4")
                                      .arg(m->ip_serv.IP[0])
                                      .arg(m->ip_serv.IP[1])
                                      .arg(m->ip_serv.IP[2])
                                      .arg(m->ip_serv.IP[3]));
          
          this->editServerPort->setText(QString::number(m->ip_serv.Port)); 
          
          this->cbSensorType->setCurrentIndex(this->cbSensorType->findData(m->sensorType));
          
          client->disconnectFromHost();
          break;
        }
          
        case SOCKET_AWAITING_CANCELED:
          log_ns::log(m_Critical, "Отменено пользователем");
          break;
          
        case SOCKET_TIMEOUT:
          log_ns::log(m_Critical, "Время ожидания ответа истекло");
          break;
          
        default:
          client->socket->abort();
          log_ns::log(m_Critical, "Операция не завершена");
          break;
          
      }
      free(m);
      break;
    }
      
    default:
      log_ns::log(m_Critical, "Не удалось подключиться к устройству");
      break;
  }
  
  delete client;
  
  setEnabledWidgets(true);
 
}

void SvDeviceConfiguration::setEnabledWidgets(bool enabled)
{
  foreach (QWidget* wdg, this->findChildren<QWidget *>())
  {
    wdg->setEnabled(enabled);
  }
}

void SvDeviceConfiguration::setupUi()
{
  this->resize(392, 277);
  vlayMain = new QVBoxLayout(this);
  vlayMain->setObjectName(QStringLiteral("vlayMain"));
  gbDeviceSettings = new QGroupBox(this);
  gbDeviceSettings->setObjectName(QStringLiteral("gbDeviceSettings"));
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(gbDeviceSettings->sizePolicy().hasHeightForWidth());
  gbDeviceSettings->setSizePolicy(sizePolicy);
  QFont font;
  font.setBold(true);
  font.setWeight(75);
  gbDeviceSettings->setFont(font);
  vlayDeviceSettings = new QVBoxLayout(gbDeviceSettings);
  vlayDeviceSettings->setObjectName(QStringLiteral("vlayDeviceSettings"));
  hlayDeviceCurrentIP = new QHBoxLayout();
  hlayDeviceCurrentIP->setObjectName(QStringLiteral("hlayDeviceCurrentIP"));
  lblDeviceCurrentIP = new QLabel(gbDeviceSettings);
  lblDeviceCurrentIP->setObjectName(QStringLiteral("lblDeviceCurrentIP"));
  lblDeviceCurrentIP->setMinimumSize(QSize(80, 0));
  QFont font1;
  font1.setBold(false);
  font1.setWeight(50);
  lblDeviceCurrentIP->setFont(font1);
  lblDeviceCurrentIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayDeviceCurrentIP->addWidget(lblDeviceCurrentIP);
 
  editDeviceCurrentIP = new QLineEdit(gbDeviceSettings);
  editDeviceCurrentIP->setObjectName(QStringLiteral("editDeviceCurrentIP"));
  editDeviceCurrentIP->setFont(font1);
 
  hlayDeviceCurrentIP->addWidget(editDeviceCurrentIP);
 
 
  vlayDeviceSettings->addLayout(hlayDeviceCurrentIP);
 
  hlayDeviceCurrentPort = new QHBoxLayout();
  hlayDeviceCurrentPort->setObjectName(QStringLiteral("hlayDeviceCurrentPort"));
  lblDeviceCurrentPort = new QLabel(gbDeviceSettings);
  lblDeviceCurrentPort->setObjectName(QStringLiteral("lblDeviceCurrentPort"));
  lblDeviceCurrentPort->setMinimumSize(QSize(80, 0));
  lblDeviceCurrentPort->setFont(font1);
  lblDeviceCurrentPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayDeviceCurrentPort->addWidget(lblDeviceCurrentPort);
 
  editDeviceCurrentPort = new QLineEdit(gbDeviceSettings);
  editDeviceCurrentPort->setObjectName(QStringLiteral("editDeviceCurrentPort"));
  editDeviceCurrentPort->setFont(font1);
 
  hlayDeviceCurrentPort->addWidget(editDeviceCurrentPort);
 
 
  vlayDeviceSettings->addLayout(hlayDeviceCurrentPort);
 
  hlayDeviceNewIP = new QHBoxLayout();
  hlayDeviceNewIP->setObjectName(QStringLiteral("hlayDeviceNewIP"));
  lblDeviceNewIP = new QLabel(gbDeviceSettings);
  lblDeviceNewIP->setObjectName(QStringLiteral("lblDeviceNewIP"));
  lblDeviceNewIP->setMinimumSize(QSize(80, 0));
  lblDeviceNewIP->setFont(font1);
  lblDeviceNewIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayDeviceNewIP->addWidget(lblDeviceNewIP);
 
  editDeviceNewIP = new QLineEdit(gbDeviceSettings);
  editDeviceNewIP->setObjectName(QStringLiteral("editDeviceNewIP"));
  editDeviceNewIP->setFont(font1);
 
  hlayDeviceNewIP->addWidget(editDeviceNewIP);
 
 
  vlayDeviceSettings->addLayout(hlayDeviceNewIP);
 
  hlayDeviceNewPort = new QHBoxLayout();
  hlayDeviceNewPort->setObjectName(QStringLiteral("hlayDeviceNewPort"));
  lblDeviceNewPort = new QLabel(gbDeviceSettings);
  lblDeviceNewPort->setObjectName(QStringLiteral("lblDeviceNewPort"));
  lblDeviceNewPort->setMinimumSize(QSize(80, 0));
  lblDeviceNewPort->setFont(font1);
  lblDeviceNewPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayDeviceNewPort->addWidget(lblDeviceNewPort);
 
  editDeviceNewPort = new QLineEdit(gbDeviceSettings);
  editDeviceNewPort->setObjectName(QStringLiteral("editDeviceNewPort"));
  editDeviceNewPort->setFont(font1);
 
  hlayDeviceNewPort->addWidget(editDeviceNewPort);
  
  vlayDeviceSettings->addLayout(hlayDeviceNewPort);
 
  hlaySensorType = new QHBoxLayout();
  hlaySensorType->setObjectName(QStringLiteral("hlaySensorType"));
  
  lblSensorType = new QLabel(gbDeviceSettings);
  lblSensorType->setObjectName(QStringLiteral("lblSensorType"));
  lblSensorType->setMinimumSize(QSize(80, 0));
  lblSensorType->setFont(font1);
  lblSensorType->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
  
  cbSensorType = new QComboBox(gbDeviceSettings);
  cbSensorType->setObjectName(QStringLiteral("cbSensorType"));
  cbSensorType->setFont(font1);
  cbSensorType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  hlaySensorType->addWidget(lblSensorType);
  hlaySensorType->addWidget(cbSensorType);
  
  vlayDeviceSettings->addLayout(hlaySensorType);
 
  vlayMain->addWidget(gbDeviceSettings);
 
  gbServerSettings = new QGroupBox(this);
  gbServerSettings->setObjectName(QStringLiteral("gbServerSettings"));
  gbServerSettings->setFont(font);
  vlayServerSettings = new QVBoxLayout(gbServerSettings);
  vlayServerSettings->setObjectName(QStringLiteral("vlayServerSettings"));
  
  hlayServerIP = new QHBoxLayout();
  hlayServerIP->setObjectName(QStringLiteral("hlayServerIP"));
  lblServerIP = new QLabel(gbServerSettings);
  lblServerIP->setObjectName(QStringLiteral("lblServerIP"));
  lblServerIP->setMinimumSize(QSize(80, 0));
  lblServerIP->setFont(font1);
  lblServerIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayServerIP->addWidget(lblServerIP);
 
  editServerIP = new QLineEdit(gbServerSettings);
  editServerIP->setObjectName(QStringLiteral("editServerIP"));
  editServerIP->setFont(font1);
 
  hlayServerIP->addWidget(editServerIP);
 
 
  vlayServerSettings->addLayout(hlayServerIP);
 
  hlayServerPort = new QHBoxLayout();
  hlayServerPort->setObjectName(QStringLiteral("hlayServerPort"));
  lblServerPort = new QLabel(gbServerSettings);
  lblServerPort->setObjectName(QStringLiteral("lblServerPort"));
  lblServerPort->setMinimumSize(QSize(80, 0));
  lblServerPort->setFont(font1);
  lblServerPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayServerPort->addWidget(lblServerPort);
 
  editServerPort = new QLineEdit(gbServerSettings);
  editServerPort->setObjectName(QStringLiteral("editServerPort"));
  editServerPort->setFont(font1);
 
  hlayServerPort->addWidget(editServerPort);
 
 
  vlayServerSettings->addLayout(hlayServerPort);
 
  hlayRequest= new QHBoxLayout();
  hlayRequest->setObjectName(QStringLiteral("hlayRequest"));
 
  bnRequest= new QPushButton(gbServerSettings);
  bnRequest->setObjectName(QStringLiteral("bnRequest"));
  bnRequest->setFont(font1);
  QIcon icon;
  icon.addFile(QStringLiteral(":/device_config/icons/cologne-icons/ico/communication.ico"), QSize(), QIcon::Normal, QIcon::Off);
  bnRequest->setIcon(icon);
  
  spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

  hlayRequest->addItem(spacer2);
  hlayRequest->addWidget(bnRequest);
  

  vlayServerSettings->addLayout(hlayRequest);
 
  vlayMain->addWidget(gbServerSettings);
 
  hlayButtons = new QHBoxLayout();
  hlayButtons->setObjectName(QStringLiteral("hlayButtons"));
  
  bnApply = new QPushButton(this);
  bnApply->setObjectName(QStringLiteral("bnApply"));
  bnApply->setFont(font);
  QIcon icon1;
  icon1.addFile(QStringLiteral(":/device_config/icons/cologne-icons/ico/check.ico"), QSize(), QIcon::Normal, QIcon::Off);
  bnApply->setIcon(icon1);
 
  spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
 
  bnCancel = new QPushButton(this);
  bnCancel->setObjectName(QStringLiteral("bnCancel"));
  bnCancel->setDefault(true);
  
  hlayButtons->addItem(spacer1);
  hlayButtons->addWidget(bnApply);
  hlayButtons->addWidget(bnCancel);
 
 
  vlayMain->addLayout(hlayButtons);

  gbDeviceSettings->setTitle(QApplication::translate("Dialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", 0));
  lblDeviceCurrentIP->setText(QApplication::translate("Dialog", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 IP", 0));
  lblDeviceCurrentPort->setText(QApplication::translate("Dialog", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 \320\277\320\276\321\200\321\202", 0));
  lblDeviceNewIP->setText(QApplication::translate("Dialog", "\320\235\320\276\320\262\321\213\320\271 IP", 0));
  lblDeviceNewPort->setText(QApplication::translate("Dialog", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\276\321\200\321\202", 0));
  lblSensorType->setText("Тип считывателя");
  gbServerSettings->setTitle(QApplication::translate("Dialog", "\320\232\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\206\320\270\321\217", 0));
  lblServerIP->setText(QApplication::translate("Dialog", "IP", 0));
  lblServerPort->setText(QApplication::translate("Dialog", "\320\237\320\276\321\200\321\202", 0));
  bnRequest->setText(""); //QApplication::translate("Dialog", "\320\227\320\260\320\277\321\200\320\276\321\201", 0));
#ifndef QT_NO_TOOLTIP
  bnRequest->setToolTip(QApplication::translate("Dialog", "\320\227\320\260\320\277\321\200\320\276\321\201\320\270\321\202\321\214 \321\202\320\265\320\272\321\203\321\211\321\203\321\216 \320\272\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\206\320\270\321\216", 0));
#endif // QT_NO_TOOLTIP
  bnApply->setText(QApplication::translate("Dialog", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214 \320\272\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\206\320\270\321\216", 0));
//  bnSave->setText(QApplication::translate("Dialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
  bnCancel->setText(QApplication::translate("Dialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
}
