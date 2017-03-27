#include "sv_employee.h"

SvEmployee* EMPLOYEE_UI;
extern SvPGDB* PGDB;
extern SvSCUD* SCUD_UI;
extern SvSelectScudkeyDialog* SELECTSCUDKEY_UI;
extern SvSelectDepartmentDialog* SELECTDEPARTMENT_UI;
extern SvDepartment* DEPARTMENT_UI;

SvEmployee::SvEmployee(QWidget *parent, int emploeeID) :
  QDialog(parent),
  ui(new Ui::SvEmploeeDialog)
{
  this->showMode = emploeeID == -1 ? smNew : smEdit;
  
  ui->setupUi(this);
  
  scudmodel = new QStandardItemModel(this);
  ui->listScudKeys->setModel(scudmodel);
  ui->listScudKeys->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db); 
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEE).arg(emploeeID), q).type())
    {  
      q->finish();
      return;
    }
    
    if(q->size())
    {
      q->first();
      t_id = q->value("employee_id").toInt();
      t_employee_name = q->value("employee_name").toString();
      t_employee_patronymic = q->value("employee_patronymic").toString();
      t_employee_surename = q->value("employee_surename").toString();
      t_short_name = q->value("short_name").toString();
      t_employee_position = q->value("employee_position").toString();
      t_birth_date = q->value("birth_date").toDate();
      t_department_id = q->value("department_id").toInt();
      t_department_name = q->value("department_name").toString();
      t_email = q->value("email").toString();
      t_phone1 = q->value("phone1").toString();
      t_phone2 = q->value("phone2").toString();
      t_description = q->value("description").toString();
      t_foto = q->value("foto").toByteArray();
    }
    
    q->finish();
    
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_SCUDKEYS_LIST)
                                           .arg(QString("where employee_id = %1").arg(emploeeID)), q).type())
    {  
      q->finish();
      return;
    }

    while(q->next())
    {
//      QString s = QString::number(q->value("key_value").toInt(), 16).toUpper();
//      s = QString(6 - s.length(), '0') + s;
      QStandardItem* item = new QStandardItem(QIcon(":/employee/icons/silk/key.png"),
                                              q->value("key_value").toString());
      item->setData(q->value("scud_id").toInt(), roleId);
      scudmodel->invisibleRootItem()->appendRow(item);
    }
    q->finish();
    
  }
  
  if(showMode == smNew) this->setWindowTitle("Новый сотрудник");
  else this->setWindowTitle(QString("Сотрудник - %1").arg(t_short_name));
  
  if(showMode == smNew) ui->editID->setText("<Новый>");
  else  ui->editID->setText(QString::number(t_id));
  
  ui->checkDeleteEmployee->setEnabled(showMode != smNew);
  
  ui->editName->setText(t_employee_name);
  ui->editPatronymic->setText(t_employee_patronymic);
  ui->editSurename->setText(t_employee_surename);
  ui->dateBirthDate->setDate(t_birth_date);
  ui->editDepartment->setText(t_department_name);
  ui->editPosition->setText(t_employee_position);
  ui->editEmail->setText(t_email);
  ui->editPhone1->setText(t_phone1);
  ui->editPhone2->setText(t_phone2);
  ui->textDescription->setPlainText(t_description);

  QPixmap pix = QPixmap();
  if(!t_foto.isEmpty()) pix.loadFromData(t_foto);
  else pix.load(":/general/2.jpg");

  ui->lblFoto->setPixmap(pix.scaled(ui->lblFoto->width(), ui->lblFoto->height(), Qt::KeepAspectRatio));
  ui->lblFoto->setScaledContents(false);
  
  connect(ui->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(ui->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(ui->bnSelectFoto, SIGNAL(clicked()), this, SLOT(loadFoto()));
  connect(ui->bnNewDepartment, SIGNAL(pressed()), this, SLOT(newDepartment()));
  connect(ui->bnSelectDepartment, SIGNAL(pressed()), this, SLOT(selectDepartment()));
  connect(ui->bnNewScudKey, SIGNAL(clicked()), this, SLOT(newScudKey()));
  connect(ui->bnSelectScudKey, SIGNAL(clicked()), this, SLOT(selectScudKey()));
  connect(ui->bnDeleteScudKey, SIGNAL(clicked()), this, SLOT(deleteScudKey()));
  
  this->setModal(true);
  this->show();  
}

SvEmployee::~SvEmployee()
{
  this->close();
  delete ui;
}

void SvEmployee::newScudKey()
{
  SCUD_UI = new SvSCUD(this);
  if(SCUD_UI->exec() == QDialog::Accepted)
  {
    QStandardItem* item = new QStandardItem(QIcon(":/employee/icons/silk/key.png"),
                                            QString::number(SCUD_UI->t_key_value));
    
    item->setData(SCUD_UI->t_id, roleId);
    scudmodel->invisibleRootItem()->appendRow(item);
  }
  SCUD_UI->~SvSCUD();
}

void SvEmployee::selectScudKey()
{
  SELECTSCUDKEY_UI = new SvSelectScudkeyDialog(this);
  if(SELECTSCUDKEY_UI->exec() == QDialog::Accepted)
  {
    QStandardItem* item = new QStandardItem(QIcon(":/employee/icons/silk/key.png"),
                                            SELECTSCUDKEY_UI->t_key_value);

    item->setData(SELECTSCUDKEY_UI->t_id, roleId);
    
    scudmodel->invisibleRootItem()->appendRow(item);
  }
  SELECTSCUDKEY_UI->~SvSelectScudkeyDialog();
  
}

void SvEmployee::deleteScudKey()
{
  scudmodel->removeRow(ui->listScudKeys->currentIndex().row());
}

void SvEmployee::newDepartment()
{
  DEPARTMENT_UI = new SvDepartment(this);
  if(DEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    t_department_id = DEPARTMENT_UI->t_id;
    t_department_name = DEPARTMENT_UI->t_department_name;
    ui->editDepartment->setText(t_department_name);
    
  }
  DEPARTMENT_UI->~SvDepartment();
}

void SvEmployee::selectDepartment()
{
  SELECTDEPARTMENT_UI = new SvSelectDepartmentDialog(this);
  if(SELECTDEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    t_department_id = SELECTDEPARTMENT_UI->t_id;
    t_department_name = SELECTDEPARTMENT_UI->t_department_name;
    ui->editDepartment->setText(t_department_name);
  }
  SELECTDEPARTMENT_UI->~SvSelectDepartmentDialog();
}

void SvEmployee::accept()
{
  if(ui->editSurename->text() == "")
  {
    QMessageBox::critical(0, "Ошибка", "Необходимо указать фамилию", QMessageBox::Ok);
    ui->editSurename->setFocus();
    return;
  }
  
  if(ui->checkDeleteEmployee->isChecked())
  {
    if(QMessageBox::question(0, "Удаление", "Вы уверены что хотите удалить сотрудника?",
                             QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
    {
      ui->checkDeleteEmployee->setChecked(false);
      return;
    }
  }
  
  t_employee_name = ui->editName->text();
  t_employee_patronymic = ui->editPatronymic->text();
  t_employee_surename = ui->editSurename->text();
  t_employee_position = ui->editPosition->text();
  t_birth_date = ui->dateBirthDate->date();
  t_email = ui->editEmail->text();
  t_phone1 = ui->editPhone1->text();
  t_phone2 = ui->editPhone2->text();

  /* формируем список id ключей */
  QStringList keys = QStringList();
  for(int i = 0; i < scudmodel->rowCount(); i++)
    keys << scudmodel->invisibleRootItem()->child(i)->data(roleId).toString();
  
  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_EMPLOYEE)
                                          .arg(t_employee_name)
                                          .arg(t_employee_patronymic)
                                          .arg(t_employee_surename)
                                          .arg(t_department_id)
                                          .arg(t_employee_position)
                                          .arg(t_birth_date.toString("dd/MM/yyyy"))
                                          .arg(t_email)
                                          .arg(t_phone1)
                                          .arg(t_phone2)
                                          .arg(t_description)
                                          .arg(keys.join(",")), &t_foto, 0, 0, q).type())
      {
        q->finish();
        QMessageBox::critical(this, "Сотрудник", "Не удалось добавить нового сотрудника", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_employee_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL (QString(SQL_UPDATE_EMPLOYEE)
                                             .arg(t_employee_name)
                                             .arg(t_employee_patronymic)
                                             .arg(t_employee_surename)
                                             .arg(t_department_id)
                                             .arg(t_employee_position)
                                             .arg(t_birth_date.toString("dd/MM/yyyy"))
                                             .arg(t_email)
                                             .arg(t_phone1)
                                             .arg(t_phone2)
                                             .arg(t_description)
                                             .arg(keys.join(","))
                                             .arg(t_id), &t_foto).type())
      {
        QMessageBox::critical(this, "Сотрудник", "Не удалось обновить данные сотрудника", QMessageBox::Ok);
        break;
      }
      
      if(ui->checkDeleteEmployee->isChecked())
      {
        if(QSqlError::NoError != PGDB->execSQL(QString("update employees set deleted = true where id = %1")
                                               .arg(t_id)).type())
        {
          QMessageBox::critical(this, "Сотрудник", "Не удалось удалить данные сотрудника", QMessageBox::Ok);
          break;
        }
      }
           
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
  
}

void SvEmployee::loadFoto()
{
  QString fn = QFileDialog::getOpenFileName(this, "Фото", qApp->applicationDirPath(), "Изображения (*.jpg *.jpeg *.png *.bmp)");
  
  if(fn == "") return;
  
  QFile *f = new QFile(fn, this);
  if(f->open(QIODevice::ReadOnly))
  {
    t_foto = f->readAll();
    f->close();
    
    QPixmap pix = QPixmap();
    pix.loadFromData(t_foto);
    ui->lblFoto->setPixmap(pix.scaled(ui->lblFoto->width(), ui->lblFoto->height(), Qt::KeepAspectRatio));
  }
}
