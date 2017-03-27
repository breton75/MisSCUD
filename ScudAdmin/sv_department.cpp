#include "sv_department.h"

SvDepartment *DEPARTMENT_UI;
extern SvPGDB *PGDB;
extern SvEmployee* EMPLOYEE_UI;

using namespace log_ns;

SvDepartment::SvDepartment(QWidget *parent, int departmentId)
//  QDialog(parent)
{
  this->showMode = departmentId == -1 ? smNew : smEdit;
  
  if(showMode == smEdit)
  {
    QSqlQuery* q = new QSqlQuery(PGDB->db);
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEPARTMENT).arg(departmentId), q).type())
    {  
      q->finish();
      delete q;
      return;
    }
    q->first();
    
    t_id = q->value("department_id").toInt();
    t_department_name = q->value("department_name").toString();
    t_boss_id = q->value("department_boss").toInt();
    t_boss_name = q->value("short_name").toString();
    t_boss_position = q->value("employee_position").toString();
    t_icon = q->value("icon").toByteArray();
    t_description = q->value("description").toString();
    
    q->finish();
    delete q;
  }
  
  setupUi();  
  
  if(showMode == smNew) this->setWindowTitle("Новый отдел");
  else this->setWindowTitle(QString("Отдел - %1").arg(t_department_name));
  
  if(showMode == smNew) editID->setText("<Новый>");
  else  editID->setText(QString::number(t_id));
  
  editDepartmentName->setText(t_department_name);
  editBossName->setText(t_boss_name); //.arg(t_boss_position));
  textDescription->setText(t_description);
  
  
  if(showMode == smNew) loadIcon(":/department/icons/cologne-icons/ico/archives.ico");
  else
  {
    QPixmap pix = QPixmap();
    pix.loadFromData(t_icon);
    lblIcon->setPixmap(pix.scaled(16, 16, Qt::KeepAspectRatio));
  }
  
  lblBossName->setEnabled(false);
  editBossName->setEnabled(false);
  bnSelectBoss->setEnabled(false);
  bnNewBoss->setEnabled(false);
  
  connect(bnSave, SIGNAL(pressed()), this, SLOT(accept()));
  connect(bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(bnSelectIcon, SIGNAL(clicked()), this, SLOT(loadIcon()));
  connect(bnNewBoss, SIGNAL(clicked()), this, SLOT(newBoss()));
  connect(bnSelectBoss, SIGNAL(clicked()), this, SLOT(selectBoss()));
  
  this->setModal(true);
  this->show();
}

SvDepartment::~SvDepartment()
{
  this->close();
  this->deleteLater();
}

void SvDepartment::newBoss()
{
  EMPLOYEE_UI = new SvEmployee(this);
  if(EMPLOYEE_UI->exec() == QDialog::Accepted)
  {
    t_boss_id = EMPLOYEE_UI->t_id;
    t_boss_name = EMPLOYEE_UI->t_short_name;
    t_boss_position = EMPLOYEE_UI->t_employee_position;
    
    editBossName->setText(t_boss_name);
  }
  EMPLOYEE_UI->~SvEmployee();
}

void SvDepartment::selectBoss()
{
  
}

void SvDepartment::accept()
{
  if(editDepartmentName->text() == "")
  {
    log_ns::log(m_Critical, "Название отдела не указано");
    editDepartmentName->setFocus();
    return;
  }
  
  t_department_name  = editDepartmentName->text();
  t_description = textDescription->toPlainText();

  bool result = false;
  switch (this->showMode)
  {
    case smNew:
    {
      QSqlQuery* q = new QSqlQuery(PGDB->db);
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_NEW_DEPARTMENT)
                                             .arg(t_department_name)
                                             .arg(t_boss_id)
                                             .arg(t_description), &t_icon, 0, 0, q).type())
      {
        q->finish();
        log_ns::log(m_Critical, "Не удалось добавить новый отдел");
        break;
      }
      q->first();
      
      t_id = q->value("new_department_id").toInt();
      qDebug() << t_id;
      q->finish();
      delete q;
      
      result = true;
      break;
    }
      
    case smEdit:
    {
      if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_DEPARTMENT)
                                             .arg(t_department_name)
                                             .arg(t_boss_id)
                                             .arg(t_description)
                                             .arg(t_id), &t_icon).type())
      {
        log_ns::log(m_Critical, "Не удалось обновить данные отдела");
        break;
      }
      
      result = true;
      break;
    }
  }

  if(result) QDialog::accept();
  else QDialog::reject();
}

void SvDepartment::loadIcon(QString fileName)
{
  QString fn;
      
  if(fileName == "")
    fn = QFileDialog::getOpenFileName(this, "Значок", qApp->applicationDirPath(), "Изображения (*.ico *.png *.bmp)");
  
  else
    fn = fileName;
  
  if(fn == "") return;
  
  QFile *f = new QFile(fn, this);
  if(f->open(QIODevice::ReadOnly))
  {
    t_icon = f->readAll();
    f->close();
  
    QPixmap pix = QPixmap();
    pix.loadFromData(t_icon);
    lblIcon->setPixmap(pix.scaled(16, 16, Qt::KeepAspectRatio));
  }
}

void SvDepartment::setupUi()
{
  this->resize(420, 240);
  vlayMain = new QVBoxLayout(this);
  vlayMain->setObjectName(QStringLiteral("vlayMain"));
  groupBox = new QGroupBox(this);
  groupBox->setObjectName(QStringLiteral("groupBox"));
  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
  groupBox->setSizePolicy(sizePolicy);
  vlayDepartment = new QVBoxLayout(groupBox);
  vlayDepartment->setSpacing(6);
  vlayDepartment->setObjectName(QStringLiteral("vlayDepartment"));
  vlayDepartment->setContentsMargins(6, 6, 6, 6);
  hlayID = new QHBoxLayout();
  hlayID->setObjectName(QStringLiteral("hlayID"));
  lblID = new QLabel(groupBox);
  lblID->setObjectName(QStringLiteral("lblID"));
  QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(0);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(lblID->sizePolicy().hasHeightForWidth());
  lblID->setSizePolicy(sizePolicy1);
  lblID->setMinimumSize(QSize(75, 0));
  lblID->setMaximumSize(QSize(75, 16777215));
  lblID->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayID->addWidget(lblID);
 
  editID = new QLineEdit(groupBox);
  editID->setObjectName(QStringLiteral("editID"));
  editID->setStyleSheet(QStringLiteral("background-color: rgb(209, 209, 209);"));
  editID->setReadOnly(true);
 
  hlayID->addWidget(editID);
 
 
  vlayDepartment->addLayout(hlayID);
 
  hlayDepartmentName = new QHBoxLayout();
  hlayDepartmentName->setObjectName(QStringLiteral("hlayDepartmentName"));
  lblDepartmentName = new QLabel(groupBox);
  lblDepartmentName->setObjectName(QStringLiteral("lblDepartmentName"));
  sizePolicy1.setHeightForWidth(lblDepartmentName->sizePolicy().hasHeightForWidth());
  lblDepartmentName->setSizePolicy(sizePolicy1);
  lblDepartmentName->setMinimumSize(QSize(75, 0));
  lblDepartmentName->setMaximumSize(QSize(75, 16777215));
  lblDepartmentName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayDepartmentName->addWidget(lblDepartmentName);
 
  editDepartmentName = new QLineEdit(groupBox);
  editDepartmentName->setObjectName(QStringLiteral("editDepartmentName"));
 
  hlayDepartmentName->addWidget(editDepartmentName);
 
  lblIcon = new QLabel(groupBox);
  lblIcon->setObjectName(QStringLiteral("lblIcon"));
  lblIcon->setMinimumSize(QSize(23, 20));
  lblIcon->setFrameShape(QFrame::Box);
  lblIcon->setFrameShadow(QFrame::Sunken);
 
  hlayDepartmentName->addWidget(lblIcon);
 
  bnSelectIcon = new QPushButton(groupBox);
  bnSelectIcon->setObjectName(QStringLiteral("bnSelectIcon"));
  QIcon icon;
  icon.addFile(QStringLiteral(":/department/icons/cologne-icons/ico/illustration.ico"), QSize(), QIcon::Normal, QIcon::Off);
  bnSelectIcon->setIcon(icon);
 
  hlayDepartmentName->addWidget(bnSelectIcon);
 
 
  vlayDepartment->addLayout(hlayDepartmentName);
 
  hlayBoss = new QHBoxLayout();
  hlayBoss->setObjectName(QStringLiteral("hlayBoss"));
  
  lblBossName = new QLabel(groupBox);
  lblBossName->setObjectName(QStringLiteral("lblBossName"));
  QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy2.setHorizontalStretch(0);
  sizePolicy2.setVerticalStretch(0);
  sizePolicy2.setHeightForWidth(lblBossName->sizePolicy().hasHeightForWidth());
  lblBossName->setSizePolicy(sizePolicy2);
  lblBossName->setMinimumSize(QSize(75, 0));
  lblBossName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
 
  hlayBoss->addWidget(lblBossName);
 
  editBossName = new QLineEdit(groupBox);
  editBossName->setObjectName(QStringLiteral("editBossName"));
  editBossName->setStyleSheet(QStringLiteral("background-color: rgb(209, 209, 209);"));
  editBossName->setReadOnly(true);
 
  hlayBoss->addWidget(editBossName);
 
  bnSelectBoss = new QPushButton(groupBox);
  bnSelectBoss->setObjectName(QStringLiteral("bnSelectBoss"));
  sizePolicy2.setHeightForWidth(bnSelectBoss->sizePolicy().hasHeightForWidth());
  bnSelectBoss->setSizePolicy(sizePolicy2);
  bnSelectBoss->setMaximumSize(QSize(26, 16777215));
  QIcon icon1;
  icon1.addFile(QStringLiteral(":/department/icons/cologne-icons/ico/publish.ico"), QSize(), QIcon::Normal, QIcon::Off);
  bnSelectBoss->setIcon(icon1);
 
  hlayBoss->addWidget(bnSelectBoss);
 
  bnNewBoss = new QPushButton(groupBox);
  bnNewBoss->setObjectName(QStringLiteral("bnNewBoss"));
  sizePolicy2.setHeightForWidth(bnNewBoss->sizePolicy().hasHeightForWidth());
  bnNewBoss->setSizePolicy(sizePolicy2);
  bnNewBoss->setMaximumSize(QSize(26, 16777215));
  QIcon icon2;
  icon2.addFile(QStringLiteral(":/department/icons/cologne-icons/ico/plus.ico"), QSize(), QIcon::Normal, QIcon::Off);
  bnNewBoss->setIcon(icon2);
 
  hlayBoss->addWidget(bnNewBoss);
 
 
  vlayDepartment->addLayout(hlayBoss);
 
  textDescription = new QTextEdit(groupBox);
  textDescription->setObjectName(QStringLiteral("textDescription"));
 
  vlayDepartment->addWidget(textDescription);
 
 
  vlayMain->addWidget(groupBox);
 
  hlayButtons = new QHBoxLayout();
  hlayButtons->setObjectName(QStringLiteral("hlayButtons"));
  spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
 
  hlayButtons->addItem(spacer1);
 
  bnSave = new QPushButton(this);
  bnSave->setObjectName(QStringLiteral("bnSave"));
  sizePolicy2.setHeightForWidth(bnSave->sizePolicy().hasHeightForWidth());
  bnSave->setSizePolicy(sizePolicy2);
  bnSave->setMaximumSize(QSize(80, 16777215));
  bnSave->setDefault(true);
 
  hlayButtons->addWidget(bnSave);
 
  bnCancel = new QPushButton(this);
  bnCancel->setObjectName(QStringLiteral("bnCancel"));
  sizePolicy2.setHeightForWidth(bnCancel->sizePolicy().hasHeightForWidth());
  bnCancel->setSizePolicy(sizePolicy2);
  bnCancel->setMaximumSize(QSize(80, 16777215));
 
  hlayButtons->addWidget(bnCancel);
 
 
  vlayMain->addLayout(hlayButtons);
 
  QWidget::setTabOrder(editDepartmentName, bnSelectIcon);
  QWidget::setTabOrder(bnSelectIcon, bnSelectBoss);
  QWidget::setTabOrder(bnSelectBoss, bnNewBoss);
  QWidget::setTabOrder(bnNewBoss, textDescription);
  QWidget::setTabOrder(textDescription, bnSave);
  QWidget::setTabOrder(bnSave, bnCancel);
  QWidget::setTabOrder(bnCancel, editID);
  QWidget::setTabOrder(editID, editBossName);

  groupBox->setTitle(QString());
  lblID->setText(QApplication::translate("SvDepartmentDialog", "ID", 0));
  lblDepartmentName->setText(QApplication::translate("SvDepartmentDialog", "\320\236\321\202\320\264\320\265\320\273", 0));
  lblIcon->setText(QString());
  bnSelectIcon->setText(QString());
  lblBossName->setText(QApplication::translate("SvDepartmentDialog", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\320\270\320\272", 0));
  bnSelectBoss->setText(QString());
  bnNewBoss->setText(QString());
  bnSave->setText(QApplication::translate("SvDepartmentDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
  bnCancel->setText(QApplication::translate("SvDepartmentDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
  
}
