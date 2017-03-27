#include "sv_select_department_dialog.h"

SvSelectDepartmentDialog *SELECTDEPARTMENT_UI;
extern SvDepartment *DEPARTMENT_UI;
extern SvPGDB *PGDB;

SvSelectDepartmentDialog::SvSelectDepartmentDialog(QWidget *parent, bool showCreate, bool showEditDelete) :
  QDialog(parent)
{
  setupUi();
    
  bnEditItem->setVisible(showEditDelete);
  bnDeleteItem->setVisible(showEditDelete);
  bnNewItem->setVisible(showCreate);
  
  connect(bnSelect, SIGNAL(pressed()), this, SLOT(accept()));
  connect(bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(bnNewItem, SIGNAL(pressed()), this, SLOT(slotNewItem()));
  connect(bnEditItem, SIGNAL(pressed()), this, SLOT(slotEditItem()));
  connect(bnDeleteItem, SIGNAL(pressed()), this, SLOT(slotDeleteItem()));  
  
  loadItems();
  
  this->setModal(true);
  this->show();
  
}

SvSelectDepartmentDialog::~SvSelectDepartmentDialog()
{
  this->close();
  this->deleteLater();
}

void SvSelectDepartmentDialog::accept()
{
  if(cbItemsList->count()) 
  {
    t_id = cbItemsList->currentData().toInt();
    t_department_name = cbItemsList->currentText();
  }  
  
  QDialog::accept();
  
}

void SvSelectDepartmentDialog::slotNewItem()
{
  DEPARTMENT_UI = new SvDepartment(this);
  if(DEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEPARTMENT_UI->t_department_name));
  }
  DEPARTMENT_UI->~SvDepartment();
}

void SvSelectDepartmentDialog::slotEditItem()
{
  if(cbItemsList->currentData().isNull()) return;
  
  DEPARTMENT_UI = new SvDepartment(this, cbItemsList->currentData().toInt());
  if(DEPARTMENT_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEPARTMENT_UI->t_department_name));
  }
  DEPARTMENT_UI->~SvDepartment();
}

void SvSelectDepartmentDialog::slotDeleteItem()
{

}

void SvSelectDepartmentDialog::loadItems()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEPARTMENTS_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  cbItemsList->addItem(QIcon(), "<Пусто>", -1);
  
  while(q->next())
  {
    QPixmap pix = QPixmap(16, 16);
    pix.loadFromData(q->value("icon").toByteArray());
    cbItemsList->addItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)),
                         q->value("department_name").toString(),
                         q->value("department_id").toInt());
  }

  q->finish();
  
  if(cbItemsList->count()) cbItemsList->setCurrentIndex(0);

}

void SvSelectDepartmentDialog::setupUi()
{
  this->resize(350, 90);
  this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); //! !!
  
  vlay = new QVBoxLayout(this);
  vlay->setObjectName(QStringLiteral("vlay"));
  
  gbItems = new QGroupBox(this);
  gbItems->setObjectName(QStringLiteral("gbItems"));
  gbItems->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  vlayItems = new QVBoxLayout(gbItems);
  vlayItems->setObjectName(QStringLiteral("vlayItems"));
  
  cbItemsList = new QComboBox(gbItems);
  cbItemsList->setObjectName(QStringLiteral("cbItemsList"));

  vlayItems->addWidget(cbItemsList);
  
  vlay->addWidget(gbItems);

  layoutButtons = new QHBoxLayout();
  layoutButtons->setObjectName(QStringLiteral("layoutButtons"));
  bnNewItem = new QPushButton(this);
  bnNewItem->setObjectName(QStringLiteral("bnNewItem"));

  layoutButtons->addWidget(bnNewItem);

  bnEditItem = new QPushButton(this);
  bnEditItem->setObjectName(QStringLiteral("bnEditItem"));

  layoutButtons->addWidget(bnEditItem);

  bnDeleteItem = new QPushButton(this);
  bnDeleteItem->setObjectName(QStringLiteral("bnDeleteItem"));

  layoutButtons->addWidget(bnDeleteItem);

  hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

  layoutButtons->addItem(hSpacer);

  bnSelect = new QPushButton(this);
  bnSelect->setObjectName(QStringLiteral("bnSelect"));

  layoutButtons->addWidget(bnSelect);

  bnCancel = new QPushButton(this);
  bnCancel->setObjectName(QStringLiteral("bnCancel"));

  layoutButtons->addWidget(bnCancel);


  vlay->addLayout(layoutButtons);

  this->setWindowTitle("Выберите производителя");
  bnNewItem->setText("Создать");
  bnEditItem->setText("Изменить");
  bnDeleteItem->setText("Удалить");
  bnSelect->setText("Выбрать");
  bnCancel->setText("Отмена");
}
