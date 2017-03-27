#include "sv_scud.h"

using namespace log_ns;

SvSCUD* SCUD_UI;
extern SvPGDB* PGDB;
//extern SvDevicesList* DEVICELIST_UI;
//extern SvZone* ZONE_UI;
//extern SvNewZone* NEW_ZONE_UI;
//extern SvZonesList* ZONELIST_UI;
extern SvEmployee* EMPLOYEE_UI;
//extern SvEmployeesList* EMPLOYEESLIST_UI;

SvSCUD::SvSCUD(QWidget *parent, int scudKeyId) :
  QDialog(parent),
  ui(new Ui::SvSCUD)
{
  this->showMode = scudKeyId == -1 ? smNew : smEdit;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_SCUDKEY).arg(scudKeyId), q).type())
    {  
      q->finish();
      return;
    }
    
    if(q->size())
    {
      q->first();
      t_id = q->value("scud_id").toInt();
      t_key_value = q->value("key_value").toInt();
      t_employee_id = q->value("employee_id").toInt();
      t_employee_name = q->value("employee_name").toString();
      t_employee_patronymic = q->value("employee_patronymic").toString();
      t_employee_surename = q->value("employee_surename").toString();
      t_employee_position = q->value("employee_position").toString();
      t_short_name = q->value("short_name").toString();
      t_device_name = q->value("device_name").toString();
//      t_zone_name = q->value("zone_name").toString();
    }
  }
  
  ui->setupUi(this);
  ui->groupBox_2->setVisible(false);
  this->resize(this->width(), this->height() - 150);
  
  if(showMode == smNew) this->setWindowTitle("Новый ключ СКУД");
  else this->setWindowTitle(QString("Ключ СКУД - %1").arg(t_key_value));
  
  if(showMode == smNew) ui->editID->setText("<Новый>");
  else  ui->editID->setText(QString::number(t_id));
  
  ui->editKey->setText(QString::number(t_key_value));
  ui->editDevice->setText(t_device_name);
  ui->editEmployee->setText(QString("%1 (%2)").arg(t_short_name).arg(t_employee_position));
//  ui->editZoneName->setText(t_zone_name);
  
  ui->editKey->setReadOnly(showMode != smNew);

  connect(ui->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(ui->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(ui->bnGetKey, SIGNAL(pressed()), this, SLOT(slotGetKey()));
  connect(ui->bnSelectEmployee, SIGNAL(pressed()), this, SLOT(on_bnSelectEmployee_clicked()));
  connect(ui->bnNewEmployee, SIGNAL(pressed()), this, SLOT(on_bnNewEmployee_clicked())); 
  connect(ui->bnSelectDevice, SIGNAL(pressed()), this, SLOT(slotSelectDevice()));
  
  this->setModal(true);
  this->show();
  
}

SvSCUD::~SvSCUD()
{
  delete ui;
}

void SvSCUD::accept()
{
  QSqlQuery* q;
  
  if((ui->editKey->text() == "") | (ui->editKey->text().toInt() == 0))
  {
    QMessageBox::critical(0, "Ошибка", "Необходимо указать ключ", QMessageBox::Ok);
    ui->editKey->setFocus();
    return;
  }
  
  
  t_key_value = ui->editKey->text().toInt();
//  t_employee_name = ui->editEmployer->text();
//  t_device_name = ui->editDevice->text();
//  t_zone_name = ui->editZoneName->text();
  
  // проверяем, что вводимый ключ, отстутствует в базе
  if(this->showMode  == smNew)
  {
    q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_CHECK_KEY_EXISTS).arg(t_key_value), q).type())
    {
      q->finish();
      return;
    }
    q->first();
    
    if(q->value("key_count").toInt() > 0)
    {
      QMessageBox::critical(this, "Ошибка",
                            QString("Такой ключ уже есть в системе"),
                            QMessageBox::Ok);
      q->finish();
      ui->editKey->setFocus();
      return;
    }
    q->finish();
  }

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_SCUDKEY)
                                             .arg(t_key_value)
                                             .arg(t_device_id)
                                             .arg(t_employee_id), q).type())
      {
        q->finish();
        QMessageBox::critical(this, "Ключи СКУД", "Не удалось добавить новый ключ СКУД", QMessageBox::Ok);
        break;
      }
      
      q->first();
      
      t_id = q->value("new_scudkey_id").toInt();
      
      q->finish();
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_SCUDKEY)
                                             .arg(t_key_value)
                                             .arg(t_device_id)
                                             .arg(t_employee_id)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Ключи СКУД", "Не удалось обновить ключ СКУД", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}

void SvSCUD::slotGetKey()
{
  awaitingKey = new AwaitingKey(this);
  if(awaitingKey->exec() == QDialog::Accepted)
  {
    ui->editKey->setText(QString("%1").arg(awaitingKey->t_key));
    log_ns::log(m_Attention, awaitingKey->t_date_time.toString("dd.MM.yy hh:mm:ss"));
  }
  awaitingKey->~AwaitingKey();
}

void SvSCUD::on_bnSelectEmployee_clicked()
{

}

void SvSCUD::on_bnNewEmployee_clicked()
{

}

void SvSCUD::slotSelectDevice()
{
//  DEVICELIST_UI = new SvDevicesList(this);
//  connect(DEVICELIST_UI, SIGNAL(accepted()), this, SLOT(on_DEVICELIST_UI_closed()));
//  connect(DEVICELIST_UI, SIGNAL(rejected()), this, SLOT(on_DEVICELIST_UI_closed()));
}


/** ********* AwaitingKey ************** **/
AwaitingKey::AwaitingKey(QWidget *parent, QString ip) :
  QDialog(parent)
{
  if (this->objectName().isEmpty())
      this->setObjectName(QStringLiteral("AwaitingKeyDialog"));
  this->setWindowModality(Qt::WindowModal);
  this->resize(330, 65);
  QVBoxLayout* verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
  QGroupBox* groupBox = new QGroupBox(this);
  groupBox->setObjectName(QStringLiteral("groupBox"));
  QVBoxLayout* verticalLayout_2 = new QVBoxLayout(groupBox);
  verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
  QHBoxLayout* horizontalLayout = new QHBoxLayout();
  horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
  QLabel* label = new QLabel(groupBox);
  label->setObjectName(QStringLiteral("label"));
  label->setText(ip == "" ? QStringLiteral("Ожидание ключа") : QStringLiteral("Ожидание ключа от ") + ip);

  horizontalLayout->addWidget(label);

  QPushButton* bnCancel = new QPushButton(groupBox);
  bnCancel->setObjectName(QStringLiteral("bnCancel"));
  bnCancel->setText(QStringLiteral("Отмена"));
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(bnCancel->sizePolicy().hasHeightForWidth());
  bnCancel->setSizePolicy(sizePolicy);

  horizontalLayout->addWidget(bnCancel);

  verticalLayout_2->addLayout(horizontalLayout);

  verticalLayout->addWidget(groupBox);
  
  connect(bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  bool b1 = sett->readValue("DEBUG", "logTcpRequests", true).toBool();
  bool b2 = sett->readValue("DEBUG", "showRequestInHex", true).toBool();
  bool b3 = sett->readValue("DEBUG", "logTcpResponses", true).toBool();
  bool b4 = sett->readValue("DEBUG", "showResponseInHex", true).toBool();  
  bool b5 = sett->readValue("DEBUG", "logTcp", true).toBool(); 
  sett->~SvSettings();
  
  tcp_server = new SvTcpServer(35580, true, this, b1, b2, b3, b4, b5);
  connect(tcp_server, SIGNAL(sigClientDisconnected()), this, SLOT(accept()));
  
  this->setModal(true);
  this->show();
  
}

AwaitingKey::~AwaitingKey()
{
  tcp_server->stopServer();
  tcp_server->deleteLater();
  
  this->close();
  this->deleteLater();
}

void AwaitingKey::accept()
{
  GotKeyStruct* gotKey = reinterpret_cast<GotKeyStruct*>(malloc(sizeof(GotKeyStruct)));
  memset(gotKey, char(0), sizeof(GotKeyStruct));
  memcpy(gotKey, tcp_server->last_message, sizeof(GotKeyStruct));
  
  /* номер считывателя */
  t_scud_number = int(gotKey->scud_number);
  t_code = t_scud_number >> 4;
  t_scud_number = t_scud_number & 0x0f;
  
  /* сам ключ */
  t_key = 0;
  memcpy(&t_key, &gotKey->key_byte1, 3);
  
  /* дата и время */
  QDate d = QDate((gotKey->year  >> 4) * 10 + (gotKey->year  & 0x0F),
                  (gotKey->month >> 4) * 10 + (gotKey->month & 0x0F),
                  (gotKey->day   >> 4) * 10 + (gotKey->day   & 0x0F));
  
  QTime t = QTime((gotKey->hour >> 4) * 10 + (gotKey->hour & 0x0F),
                  (gotKey->min  >> 4) * 10 + (gotKey->min  & 0x0F),
                  (gotKey->sec  >> 4) * 10 + (gotKey->sec  & 0x0F));

  t_date_time.setDate(d);
  t_date_time.setTime(t);
  
  /* день недели */
//  t_weekday = gotKey->weekday;
  
  free(gotKey);
   
  QDialog::accept();
  
}

void AwaitingKey::reject()
{
  QDialog::reject();
}

