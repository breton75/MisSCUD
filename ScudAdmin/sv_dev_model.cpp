#include "sv_dev_model.h"
#include "ui_devicemodel.h"

SvDevModel *DEVMODEL_UI;
extern SvBrand *DEVBRAND_UI;
extern SvDeviceClass *DEVCLASS_UI;
extern SvSelectBrandDialog *SELECTBRAND_UI;
extern SvSelectClassDialog *SELECTCLASS_UI;
extern SvPGDB *PGDB;

SvDevModel::SvDevModel(QWidget *parent, int modelId, int showMode) :
  QDialog(parent),
  dmDialog(new Ui::SvDeviceModelDialog) 
{
  this->showMode = showMode;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_MODEL).arg(modelId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("device_id").toInt();
    t_brand_id = q->value("brand_id").toInt();
    t_class_id = q->value("class_id").toInt();
    t_model_name = q->value("model_name").toString();
    t_brand_name = q->value("brand_name").toString();
    t_class_name = q->value("class_name").toString();
    t_driver_path = q->value("model_lib_path").toString();
    t_description = q->value("description").toString();
    
    q->finish();
    delete q;
  }
  
  dmDialog->setupUi(this); 
  
  if(showMode == smNew) this->setWindowTitle("Новая модель");
  else this->setWindowTitle(QString("Модель - %1").arg(t_model_name));
  
  if(showMode == smNew) dmDialog->editID->setText("<Новый>");
  else  dmDialog->editID->setText(QString::number(t_id));
  
  dmDialog->editModelName->setText(t_model_name);
  dmDialog->editClass->setText(t_class_name);
  dmDialog->editBrand->setText(t_brand_name);
  dmDialog->editLibPath->setText(t_driver_path);        
  dmDialog->textDescription->setText(t_description);
  
  connect(dmDialog->bnOk, SIGNAL(pressed()), this, SLOT(accept()));
  connect(dmDialog->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  
  connect(dmDialog->bnNewBrand, SIGNAL(pressed()), this, SLOT(slotNewBrand()));
  connect(dmDialog->bnNewClass, SIGNAL(pressed()), this, SLOT(slotNewClass()));
  connect(dmDialog->bnSelectBrand, SIGNAL(pressed()), this, SLOT(slotSelectBrand()));
  connect(dmDialog->bnSelectClass, SIGNAL(pressed()), this, SLOT(slotSelectClass()));
  connect(dmDialog->bnLibPath, SIGNAL(pressed()), this, SLOT(slotSelectLibPath()));
  
  this->setModal(true);
  this->show();
  
}

SvDevModel::~SvDevModel()
{
  this->close();
  delete dmDialog;
}

void SvDevModel::accept()
{
  /* делаем всякие проверки вводимых данных */
  if(dmDialog->editModelName->text().isEmpty())
  {
    QMessageBox::critical(0, "Error", "Model name not set", QMessageBox::Ok);
    dmDialog->editModelName->setFocus();
    return;
  }
  
  if(dmDialog->editLibPath->text().isEmpty())
  {
    QMessageBox::critical(0, "Error", "Driver library path not set", QMessageBox::Ok);
    dmDialog->editLibPath->setFocus();
    return;
  }  
  /* конец проверок */  

  t_id          = dmDialog->editID->text().toInt();
  t_model_name  = dmDialog->editModelName->text();
  t_brand_name  = dmDialog->editBrand->text();
//  t_class_name  = dmDialog->comboBox->currentText();  //editClass->text(); 
  t_class_name  = dmDialog->editClass->text();
  t_driver_path = dmDialog->editLibPath->text();
  t_description = dmDialog->textDescription->toPlainText();

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_MODEL)
                                             .arg(t_model_name)
                                             .arg(t_class_id)
                                             .arg(t_brand_id)
                                             .arg(t_driver_path)
                                             .arg(t_description), q).type())
      {
        q->finish();
        delete q;
        QMessageBox::critical(this, "Модель", "Не удалось добавить новую модель", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_model_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_MODEL)
                                             .arg(t_model_name)
                                             .arg(t_class_id)
                                             .arg(t_brand_id)
                                             .arg(t_driver_path)
                                             .arg(t_description)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Модель", "Не удалось обновить данные", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}

void SvDevModel::slotNewBrand()
{
  DEVBRAND_UI = new SvBrand(this);
  if(DEVBRAND_UI->exec() == QDialog::Accepted)
  {
    t_brand_id = DEVBRAND_UI->t_id;
    dmDialog->editBrand->setText(DEVBRAND_UI->t_brand_name);
  }
  
  DEVBRAND_UI->~SvBrand();
}

void SvDevModel::slotNewClass()
{
  DEVCLASS_UI = new SvDeviceClass(this);
  if(DEVCLASS_UI->exec() == QDialog::Accepted)
  {
    t_class_id = DEVCLASS_UI->t_id;
    dmDialog->editClass->setText(DEVCLASS_UI->t_class_name);
  }
  DEVCLASS_UI->~SvDeviceClass();  
}

void SvDevModel::slotSelectBrand()
{
  SELECTBRAND_UI = new SvSelectBrandDialog(this);
  if(SELECTBRAND_UI->exec() == QDialog::Accepted)
  {
    t_brand_id = SELECTBRAND_UI->t_id;
    dmDialog->editBrand->setText(SELECTBRAND_UI->t_brand_name);
  }
  SELECTBRAND_UI->~SvSelectBrandDialog();
}

void SvDevModel::slotSelectClass()
{
  SELECTCLASS_UI = new SvSelectClassDialog(this);
  if(SELECTCLASS_UI->exec() == QDialog::Accepted)
  {
    t_class_id = SELECTCLASS_UI->t_id;
    dmDialog->editClass->setText(SELECTCLASS_UI->t_class_name);
  }
  SELECTCLASS_UI->~SvSelectClassDialog();
}

void SvDevModel::slotSelectLibPath()
{
  dmDialog->editLibPath->setText(QFileDialog::getOpenFileName(this, "Выбор файла драйвера для модели", qApp->applicationDirPath()));
}


