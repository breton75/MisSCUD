#include "sv_dev_class.h"

SvDeviceClass *DEVCLASS_UI;

extern SvPGDB *PGDB;

SvDeviceClass::SvDeviceClass(QWidget *parent, int classId, int showMode) :
  
  QDialog(parent),
  Dialog(new Ui::SvClassDialog)
{
  this->showMode = showMode;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_CLASS).arg(classId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("class_id").toInt();
    t_class_name = q->value("class_name").toString();
    t_driver_path = q->value("class_lib_path").toString();
    t_icon = q->value("icon").toByteArray();
    t_description = q->value("description").toString();
    
    q->finish();
    delete q;
  }
  
  Dialog->setupUi(this);   
  
  if(showMode == smNew) this->setWindowTitle("Новый класс устройств");
  else this->setWindowTitle(QString("Класс устройств - %1").arg(t_class_name));
  
  if(showMode == smNew) Dialog->editID->setText("<Новый>");
  else  Dialog->editID->setText(QString::number(t_id));
  
  Dialog->editClassName->setText(t_class_name);
  Dialog->editLibPath->setText(t_driver_path);
  Dialog->textDescription->setText(t_description);
//  if(!t_icon.isEmpty()) Dialog->lblIconPreview->setPixmap(pix);
  QPixmap pix = QPixmap(16, 16);
  if(showMode == smNew) pix.load(":/tree/icons/cologne-icons/ico/library.ico");
  else  pix.loadFromData(t_icon);
  Dialog->lblIconPreview->setPixmap(pix);
  
  Dialog->editClassName->setReadOnly(showMode != smNew);
  
  connect(Dialog->bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(Dialog->bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(Dialog->bnIconPath, SIGNAL(clicked()), this, SLOT(selectIcon()));
  connect(Dialog->bnLibPath, SIGNAL(pressed()), this, SLOT(slotSelectLibPath()));
  
  this->setModal(true);
  this->show();

}

SvDeviceClass::~SvDeviceClass()
{
  this->close();
  delete Dialog;
}

void SvDeviceClass::accept()
{
  if(Dialog->editClassName->text() == "")
  {
    Dialog->editClassName->setFocus();
    return;
  }
  
  t_id =  Dialog->editID->text().toInt();
  t_class_name  = Dialog->editClassName->text();
  t_driver_path = Dialog->editLibPath->text();
  t_description = Dialog->textDescription->toPlainText();

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_CLASS)
                                             .arg(t_class_name)
                                             .arg(t_driver_path)
                                             .arg(t_description), &t_icon, 0, 0, q).type())
      {
        q->finish();
        delete q;
        QMessageBox::critical(this, "Класс устройств", "Не удалось добавить новый класс устройств", QMessageBox::Ok);
        break;
      }
      q->first();
      
      t_id = q->value("new_class_id").toInt();
      
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_CLASS)
                                             .arg(t_class_name)
                                             .arg(t_driver_path)
                                             .arg(t_description)
                                             .arg(t_id), &t_icon).type())
      {
        QMessageBox::critical(this, "Класс устройств", "Не удалось обновить класс устройств", QMessageBox::Ok);
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
}

void SvDeviceClass::selectIcon()
{
  QString fn = QFileDialog::getOpenFileName(this, "Значок", qApp->applicationDirPath(), "Изображения (*.ico *.png *.bmp)");
  if(fn == "") return;
  
  QFile *f = new QFile(fn, this);
  if(f->open(QIODevice::ReadOnly))
  {
    t_icon = f->readAll();
    f->close();
  
    QPixmap pix = QPixmap(16, 16);
    pix.loadFromData(t_icon);
    Dialog->lblIconPreview->setPixmap(pix);
  }
}

void SvDeviceClass::slotSelectLibPath()
{
  Dialog->editLibPath->setText(QFileDialog::getOpenFileName(this, "Выбор файла драйвера для класса устройств", qApp->applicationDirPath()));
}
