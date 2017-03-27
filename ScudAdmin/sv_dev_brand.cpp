#include "sv_dev_brand.h"
//#include "ui_device.h"

SvBrand *DEVBRAND_UI;
extern SvPGDB *PGDB;

SvBrand::SvBrand(QWidget *parent, int brandId) :
  QDialog(parent),
  Dialog(new Ui::SvBrandDialog)
{
  this->showMode = brandId == -1 ? smNew : smEdit;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_BRAND).arg(brandId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("brand_id").toInt();
    t_brand_name = q->value("brand_name").toString();
    t_description = q->value("description").toString();
    
    q->finish();
    delete q;
  }
  
  Dialog->setupUi(this);  
  
  if(showMode == smNew) this->setWindowTitle("Новый производитель");
  else this->setWindowTitle(QString("Производитель - %1").arg(t_brand_name));
  
  if(showMode == smNew) Dialog->editID->setText("<Новый>");
  else  Dialog->editID->setText(QString::number(t_id));
  
  Dialog->editBrandName->setText(t_brand_name);
  Dialog->textDescription->setText(t_description);
  
//  Dialog->editBrandName->setReadOnly(showMode != smNew);
  
  Dialog->editAddress->setEnabled(false);
  Dialog->editEmail->setEnabled(false);
  Dialog->editSite->setEnabled(false);
    
  connect(Dialog->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(Dialog->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  
  this->setModal(true);
  this->show();

}

SvBrand::~SvBrand()
{
  this->close();
  delete Dialog;
}

void SvBrand::accept()
{
  t_brand_name = Dialog->editBrandName->text();
  t_description = Dialog->textDescription->toPlainText();
  
  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_BRAND)
                                             .arg(t_brand_name)
                                             .arg(t_description), q).type())
      {
        q->finish();
        delete q;
        QMessageBox::critical(this, "Производитель", "Не удалось добавить нового производителя", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_brand_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_BRAND)
                                             .arg(t_brand_name)
                                             .arg(t_description)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Производитель", "Не удалось обновить данные", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();

}

