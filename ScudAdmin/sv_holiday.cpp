#include "sv_holiday.h"

SvHoliday *HOLIDAY_UI;

extern SvPGDB *PGDB;

SvHoliday::SvHoliday(QWidget *parent, int holidayId, int showMode) :
  
  QDialog(parent),
  Dialog(new Ui::SvHolidayDialog)
{
  this->showMode = showMode;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_HOLIDAY).arg(holidayId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("holiday_id").toInt();
    t_holiday_name = q->value("holiday_name").toString();
    
    q->finish();
    delete q;
  }
  
  Dialog->setupUi(this);   
  
  if(showMode == smNew) this->setWindowTitle("Новый праздник");
  else this->setWindowTitle(QString("Праздник - %1").arg(t_holiday_name));
  
  if(showMode == smNew) Dialog->editID->setText("<Новый>");
  else  Dialog->editID->setText(QString::number(t_id));
  
  Dialog->editHolidayName->setText(t_holiday_name);
//  if(!t_icon.isEmpty()) Dialog->lblIconPreview->setPixmap(pix);
  
  connect(Dialog->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(Dialog->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  
  this->setModal(true);
  this->show();

}

SvHoliday::~SvHoliday()
{
  this->close();
  delete Dialog;
}

void SvHoliday::accept()
{
  if(Dialog->editHolidayName->text() == "")
  {
    Dialog->editHolidayName->setFocus();
    return;
  }
  
  t_id =  Dialog->editID->text().toInt();
  t_holiday_name = Dialog->editHolidayName->text();

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_HOLIDAY)
                                             .arg(t_holiday_name), q).type())
      {
        q->finish();
        delete q;
        QMessageBox::critical(this, "Праздник", "Не удалось добавить новый праздник", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_holiday_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_HOLIDAY)
                                             .arg(t_holiday_name)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Праздник", "Не удалось обновить праздник", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
}
