#include "sv_trackersettings.h"

SvTrackerSettings* TRACKERSETTINGS_UI;
extern SvSelectDeviceDialog *SELECTDEVICEDIALOG_UI;
extern SvPGDB *PGDB;

SvTrackerSettings::SvTrackerSettings(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SvTrackerSettings)
{
  ui->setupUi(this);
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_TRACKING_DEVICES_LIST)
                                            .arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    ui->tblDevices->insertRow(ui->tblDevices->rowCount());
    ui->tblDevices->setRowHeight(ui->tblDevices->rowCount() - 1, 20);
    
    QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1 (%2 %3:%4)")
                                                     .arg(q->value("device_name").toString())
                                                     .arg(q->value("model_name").toString())
                                                     .arg(q->value("ip").toString())
                                                     .arg(q->value("port").toInt()));
    
    newItem->setData(roleId, SELECTDEVICEDIALOG_UI->t_id);
    newItem->setFlags(Qt::ItemIsEnabled);
    ui->tblDevices->setItem(ui->tblDevices->rowCount() - 1, 0, newItem);
    
    
    newItem = new QTableWidgetItem(q->value("period").toString());
    ui->tblDevices->setItem(ui->tblDevices->rowCount() - 1, 1, newItem);
  }
  q->finish();
  
  connect(ui->bnAddDevice, SIGNAL(clicked()), this, SLOT(addDevice()));
  connect(ui->bnDeleteDevice, SIGNAL(clicked()), this, SLOT(deleteDevice()));
  connect(ui->bnSave, SIGNAL(clicked()), this, SLOT(accept()));
  connect(ui->bnCancel, SIGNAL(clicked()), this, SLOT(reject()));
      
  this->setModal(true);
  this->show();
  
}

SvTrackerSettings::~SvTrackerSettings()
{
  delete ui;
}

void SvTrackerSettings::addDevice()
{
  SELECTDEVICEDIALOG_UI = new SvSelectDeviceDialog(this);
  if(SELECTDEVICEDIALOG_UI->exec() == QDialog::Accepted)
  {
    ui->tblDevices->insertRow(ui->tblDevices->rowCount());
    ui->tblDevices->setRowHeight(ui->tblDevices->rowCount() - 1, 20);
    
    QTableWidgetItem *newItem = new QTableWidgetItem(SELECTDEVICEDIALOG_UI->t_device_name);
    
    newItem->setData(roleId, SELECTDEVICEDIALOG_UI->t_id);
    newItem->setFlags(Qt::ItemIsEnabled);
    ui->tblDevices->setItem(ui->tblDevices->rowCount() - 1, 0, newItem);
    
    newItem = new QTableWidgetItem("1000");
    newItem->setData(roleId, SELECTDEVICEDIALOG_UI->t_id);
    ui->tblDevices->setItem(ui->tblDevices->rowCount() - 1, 1, newItem);

  }
  SELECTDEVICEDIALOG_UI->~SvSelectDeviceDialog();
}

void SvTrackerSettings::deleteDevice()
{
  ui->tblDevices->removeRow(ui->tblDevices->currentRow());
}

void SvTrackerSettings::accept()
{
  /* проверяем правильность ввода чисел */
  for(int i = 0; i < ui->tblDevices->rowCount(); i ++)
  {
    if(!ui->tblDevices->item(i, 1)->text().toInt())
    {
      QMessageBox::critical(this, "Ошибка", "Неверное число", QMessageBox::Ok);
      ui->tblDevices->setCurrentCell(i, 1);
      return;
    }
  }
  
  
  
  
  QDialog::accept();
}
