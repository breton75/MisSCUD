#include "sv_device.h"

SvDevice *DEVICE_UI;
extern SvDevModel* DEVMODEL_UI;
extern SvPGDB *PGDB;
extern SvSelectModelDialog* SELECTMODELDIALOG_UI;
extern SvDeviceConfiguration* DEVICE_CONFIG_UI;

using namespace log_ns;

//extern SvZone* ZONE_UI;
//extern SvNewZone* NEW_ZONE_UI;
//extern SvZonesList *ZONELIST_UI;

SvDevice::SvDevice(QWidget *parent, int deviceId) :
  QDialog(parent),
  Dialog(new Ui::SvDeviceDialog)
{
  this->showMode = deviceId == -1 ? smNew : smEdit;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEVICE).arg(deviceId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("device_id").toInt();
    t_device_name = q->value("device_name").toString();
    t_model_id = q->value("model_id").toInt();
    t_brand_id = q->value("brand_id").toInt();
    t_class_id = q->value("class_id").toInt();
    t_model_name = q->value("model_name").toString();
    t_brand_name = q->value("brand_name").toString();
    t_class_name = q->value("class_name").toString();
    t_ip = q->value("ip").toInt();
    t_port = q->value("port").toInt();
    t_driver_path = q->value("model_lib_path").toString();
    t_description = q->value("description").toString();
    
    q->finish();
    delete q;
  }
  
  Dialog->setupUi(this);  
  
  if(showMode == smNew) this->setWindowTitle("Новое устройство");
  else this->setWindowTitle(QString("Устройство - %1").arg(t_device_name));
  
  if(showMode == smNew) Dialog->editID->setText("<Новый>");
  else  Dialog->editID->setText(QString::number(t_id));
  
  Dialog->editDevName->setText(t_device_name);
  Dialog->editIP->setText(QHostAddress(t_ip).toString());
  Dialog->editPort->setText(QString::number(t_port));
  Dialog->editModelName->setText(t_model_name);
  Dialog->editClass->setText(t_class_name);
  Dialog->editBrand->setText(t_brand_name);
  Dialog->editDriver->setText(t_driver_path);
  Dialog->textDescription->setText(t_description);
  
  Dialog->editDevName->setReadOnly(showMode != smNew);
  Dialog->bnConfig->setEnabled(t_model_id != -1);
  
  connect(Dialog->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(Dialog->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(Dialog->bnNewModel, SIGNAL(pressed()), this, SLOT(slotNewModel()));
  connect(Dialog->bnSelectModel, SIGNAL(pressed()), this, SLOT(slotSelectModel()));
  connect(Dialog->bnConfig, SIGNAL(clicked()), this, SLOT(config()));
  
  this->setModal(true);
  this->show();
}

SvDevice::~SvDevice()
{
  this->close();
  delete Dialog;
}

void SvDevice::accept()
{
  /* делаем всякие проверки вводимых данных */
  if(Dialog->editDevName->text() == "")
  {
    log_ns::log(m_Critical, "Имя устройства не указано");
    Dialog->editDevName->setFocus();
    return;
  }
  
  if(QHostAddress(Dialog->editIP->text()).toIPv4Address() == 0)
  {
    log_ns::log(m_Critical, "IP адрес указан неверно");
    Dialog->editIP->setFocus();
    return;
  }  
  
  if((Dialog->editPort->text() == "") || (Dialog->editPort->text().toInt() == 0))
  {
    log_ns::log(m_Critical, "Порт указан неверно");
    Dialog->editPort->setFocus();
    return;
  }   
  
  if(t_model_id == -1)
  {
    log_ns::log(m_Critical, "Необходимо указать модель устройства");
    Dialog->bnSelectModel->setFocus();
    return;
  }
  
  /* проверяем уникальность ip адреса */
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_CHECK_IP_EXISTS)
                                         .arg(qint32(QHostAddress(Dialog->editIP->text()).toIPv4Address())), q).type())
  {
    q->finish();
    log_ns::log(m_Critical, "Ошибка вызова функции БД check_ip_exists(int)");
    return;
  }
  q->first();
  bool found = q->value("cnt").toInt() > 0;
  q->finish();
  
  if(found)
  {
    log_ns::log(m_Critical, QString("Устройство с IP адресом %1 уже есть в системе")
                              .arg(Dialog->editIP->text()));
    Dialog->editIP->setFocus();
    return;
  }
  
  /* конец проверок */
  
  t_device_name = Dialog->editDevName->text();
  t_ip = QHostAddress(Dialog->editIP->text()).toIPv4Address(); //  Dialog->editIP->text();
  t_port = Dialog->editPort->text().toInt();
//  t_model_name = Dialog->editModelName->text();
//  t_class_name = Dialog->editClass->text();
//  t_brand_name = Dialog->editBrand->text();
  t_driver_path = Dialog->editDriver->text();
  t_description = Dialog->textDescription->toPlainText();

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_DEVICE)
                                             .arg(t_device_name)
                                             .arg(t_model_id)
                                             .arg(t_class_id)
                                             .arg(t_brand_id)
                                             .arg(t_ip)
                                             .arg(t_port)
                                             .arg(t_description), q).type())
      {
        q->finish();
        delete q;
        QMessageBox::critical(this, "Устройство", "Не удалось добавить новое устройство", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_device_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_DEVICE)
                                             .arg(t_device_name)
                                             .arg(t_model_id)
                                             .arg(t_class_id)
                                             .arg(t_brand_id)
                                             .arg(t_ip)
                                             .arg(t_port)
                                             .arg(t_description)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Устройство", "Не удалось обновить данные", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}

void SvDevice::config()
{
  DEVICE_CONFIG_UI = new SvDeviceConfiguration(t_id);
//  DEVICE_CONFIG_UI->setCurrentIP(Dialog->editIP->text() == "" ? "192.168.3.31" : Dialog->editIP->text());
//  DEVICE_CONFIG_UI->setCurrentPort(Dialog->editPort->text().toInt() == 0 ? 35580 : Dialog->editPort->text().toInt());
  DEVICE_CONFIG_UI->setCurrentData(Dialog->editIP->text(), Dialog->editPort->text().toInt());
  if(DEVICE_CONFIG_UI->exec() == QDialog::Accepted)
  {
    Dialog->editIP->setText(DEVICE_CONFIG_UI->t_device_new_ip);
    Dialog->editPort->setText(QString::number(DEVICE_CONFIG_UI->t_device_new_port));
  }
  DEVICE_CONFIG_UI->~SvDeviceConfiguration();
}

void SvDevice::slotSelectModel()
{
  SELECTMODELDIALOG_UI = new SvSelectModelDialog(this);
  if(SELECTMODELDIALOG_UI->exec() == QDialog::Accepted)
  {
    t_model_id = SELECTMODELDIALOG_UI->t_id;
    t_class_id = SELECTMODELDIALOG_UI->t_class_id; 
    t_brand_id = SELECTMODELDIALOG_UI->t_brand_id; 
    t_model_name = SELECTMODELDIALOG_UI->t_model_name;
    t_class_name = SELECTMODELDIALOG_UI->t_class_name;
    t_brand_name = SELECTMODELDIALOG_UI->t_brand_name;
    t_driver_path = SELECTMODELDIALOG_UI->t_driver_path;
    
    Dialog->editModelName->setText(t_model_name);
    Dialog->editClass->setText(t_class_name);
    Dialog->editBrand->setText(t_brand_name);
    Dialog->editDriver->setText(t_driver_path);
  }
  SELECTMODELDIALOG_UI->~SvSelectModelDialog();
  
  Dialog->bnConfig->setEnabled(t_model_id != -1);
}

void SvDevice::slotNewModel()
{
  DEVMODEL_UI = new SvDevModel(this);
  if(DEVMODEL_UI->exec() == QDialog::Accepted)
  {
    t_model_id = DEVMODEL_UI->t_id;
    t_class_id = DEVMODEL_UI->t_class_id; 
    t_brand_id = DEVMODEL_UI->t_brand_id; 
    t_model_name = DEVMODEL_UI->t_model_name;
    t_class_name = DEVMODEL_UI->t_class_name;
    t_brand_name = DEVMODEL_UI->t_brand_name;
    t_driver_path = DEVMODEL_UI->t_driver_path;
    
    Dialog->editModelName->setText(t_model_name);
    Dialog->editClass->setText(t_class_name);
    Dialog->editBrand->setText(t_brand_name);
    Dialog->editDriver->setText(t_driver_path);
  }
  DEVMODEL_UI->~SvDevModel();
  
  Dialog->bnConfig->setEnabled(t_model_id != -1);
}

