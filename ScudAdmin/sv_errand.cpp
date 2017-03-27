#include "sv_errand.h"

SvErrand* ERRAND_UI;
extern SvPGDB* PGDB;

SvErrand::SvErrand(QWidget *parent, int errandId) :
  QDialog(parent),
  ui(new Ui::SvErrandDialog)
{
  ui->setupUi(this);
  
  showMode = errandId == -1 ? smNew : smEdit;
           
  loadEmployees();
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_ERRAND_JOURNAL).arg(errandId), q).type())  
    {
      q->finish();
      delete q;
      return;
    }
   
    if(q->next())
    {
      t_id = errandId;
      t_employee_id = q->value("employee_id").toInt();
      t_errand_begin = q->value("errand_begin").toDateTime();
      t_errand_end = q->value("errand_end").toDateTime();
      t_destination = q->value("destination").toString();
      t_description = q->value("description").toString();
    }
    
    q->finish();
    delete q;
    
    ui->cbEmployee->setCurrentIndex(ui->cbEmployee->findData(t_employee_id));
        
  }
  
  ui->dateDate->setDate(t_errand_begin.date());
  ui->timeTimeBegin->setTime(t_errand_begin.time());
  ui->timeTimeEnd->setTime(t_errand_end.time());
  ui->editDestination->setText(t_destination);
  ui->textDescription->setText(t_description);
  
  connect(ui->bnSave, SIGNAL(clicked()), this, SLOT(accept()));
  connect(ui->bnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  
  this->setModal(true);
  this->show();
  
}

SvErrand::~SvErrand()
{
  delete ui;
}

void SvErrand::loadEmployees()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEES_LIST_LIGHT)
                                         .arg("where deleted = false"), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
    ui->cbEmployee->addItem(QString("%1 (%2)")
                              .arg(q->value("short_name").toString())
                              .arg(q->value("employee_position").toString()),
                         q->value("employee_id").toInt());

  q->finish();
  delete q;
  
  if(ui->cbEmployee->count()) ui->cbEmployee->setCurrentIndex(0);
  
  ui->bnSave->setEnabled(!ui->cbEmployee->currentData().isNull());
  
}

void SvErrand::accept()
{
  if(!ui->dateDate->date().isValid() ||
     !ui->timeTimeBegin->time().isValid() ||
     !ui->timeTimeEnd->time().isValid() ||
     (ui->timeTimeBegin->time() > ui->timeTimeEnd->time()))
  {
    QMessageBox::critical(this, "Ошибка", "Неверно указаны начало и конец периода", QMessageBox::Ok);
    ui->dateDate->setFocus();
    return;
  }
  
  if(ui->editDestination->text() == "")
  {
    QMessageBox::critical(this, "Ошибка", "Не указано место командировки", QMessageBox::Ok);
    ui->dateDate->setFocus();
    return;
  }  
  
  t_employee_id = ui->cbEmployee->currentData().toInt();
  t_errand_begin.setDate(ui->dateDate->date());
  t_errand_end.setDate(ui->dateDate->date());
  
  t_errand_begin.setTime(ui->timeTimeBegin->time());
  t_errand_end.setTime(ui->timeTimeEnd->time());
    
  t_destination = ui->editDestination->text();
  t_description = ui->textDescription->toPlainText();
  
  
  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_ERRAND_JOURNAL)
                                          .arg(t_employee_id)
                                          .arg(t_errand_begin.toString("dd/MM/yyyy hh:mm"))
                                          .arg(t_errand_end.toString("dd/MM/yyyy hh:mm"))
                                          .arg(t_destination)
                                          .arg(t_description), q).type())
      {
        q->finish();
        QMessageBox::critical(this, "Запись в журнал", "Не удалось добавить новую запись в журнал местных командировок", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_errand_journal_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
      
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_ERRAND_JOURNAL)
                                             .arg(t_employee_id)
                                             .arg(t_errand_begin.toString("dd/MM/yyyy hh:mm:ss"))
                                             .arg(t_errand_end.toString("dd/MM/yyyy hh:mm:ss"))
                                             .arg(t_destination)
                                             .arg(t_description)
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Запись в журнал", "Не удалось обновить запись в журнале местных командировок", QMessageBox::Ok);
        break;
      }      
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}
