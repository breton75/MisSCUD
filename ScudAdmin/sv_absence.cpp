#include "sv_absence.h"
#include "ui_sv_absence.h"

SvAbsence* ABSENCE_UI;
extern SvPGDB* PGDB;

SvAbsence::SvAbsence(QWidget *parent, int absenceId) :
  QDialog(parent),
  ui(new Ui::SvAbsenceDialog)
{
  ui->setupUi(this);
  
  showMode = absenceId == -1 ? smNew : smEdit;
           
  loadEmployees();
  loadAbsences();
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_ABSENCE_JOURNAL).arg(absenceId), q).type())
    {
      q->finish();
      delete q;
      return;
    }
    
    q->first();
    
    t_id = absenceId;
    t_period_begin = q->value("absence_begin").toDate();
    t_period_end = q->value("absence_end").toDate();
    t_absence_id = q->value("absence_id").toInt();
    t_employee_id = q->value("employee_id").toInt();
    
    q->finish();
    delete q;
    
    ui->cbAbsence->setCurrentIndex(ui->cbAbsence->findData(t_absence_id));
    ui->cbEmployee->setCurrentIndex(ui->cbEmployee->findData(t_employee_id));
        
  }
  
  ui->datePeriodBegin->setDate(t_period_begin);
  ui->datePeriodEnd->setDate(t_period_end);
  
  connect(ui->bnSave, SIGNAL(clicked()), this, SLOT(accept()));
  connect(ui->bnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  
  this->setModal(true);
  this->show();
  
}

SvAbsence::~SvAbsence()
{
  delete ui;
}

void SvAbsence::loadEmployees()
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

void SvAbsence::loadAbsences()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_ABSENCES_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
    ui->cbAbsence->addItem(QString("%1 (%2)")
                              .arg(q->value("absence_name").toString())
                              .arg(q->value("designation").toString()),
                         q->value("absence_id").toInt());

  q->finish();
  
  if(ui->cbAbsence->count()) ui->cbAbsence->setCurrentIndex(0);
  
  ui->bnSave->setEnabled(!ui->cbAbsence->currentData().isNull());
}

void SvAbsence::accept()
{
  if(!ui->datePeriodBegin->date().isValid() ||
     !ui->datePeriodEnd->date().isValid() ||
     (ui->datePeriodBegin->date() > ui->datePeriodEnd->date()))
  {
    QMessageBox::critical(this, "Ошибка", "Неверно указаны даты периода", QMessageBox::Ok);
    ui->datePeriodBegin->setFocus();
    return;
  }
  
  t_absence_id = ui->cbAbsence->currentData().toInt();
  t_employee_id = ui->cbEmployee->currentData().toInt();
  t_period_begin = ui->datePeriodBegin->date();
  t_period_end = ui->datePeriodEnd->date();
  
  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_ABSENCE_JOURNAL)
                                          .arg(t_employee_id)
                                          .arg(t_absence_id)
                                          .arg(t_period_begin.toString("dd/MM/yyyy"))
                                          .arg(t_period_end.toString("dd/MM/yyyy")), q).type())
      {
        q->finish();
        QMessageBox::critical(this, "Запись в журнал", "Не удалось добавить новую запись в журнал отсутствий", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_absence_journal_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
      
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_ABSENCE_JOURNAL)
                                             .arg(t_employee_id)
                                             .arg(t_absence_id)
                                             .arg(t_period_begin.toString("dd/MM/yyyy"))
                                             .arg(t_period_end.toString("dd/MM/yyyy"))
                                             .arg(t_id)).type())
      {
        QMessageBox::critical(this, "Запись в журнал", "Не удалось обновить запись в журнале отсутствий", QMessageBox::Ok);
        break;
      }      
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}
