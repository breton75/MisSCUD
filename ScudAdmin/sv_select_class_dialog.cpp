#include "sv_select_class_dialog.h"

SvSelectClassDialog *SELECTCLASS_UI;
extern SvDeviceClass *DEVCLASS_UI;
extern SvPGDB *PGDB;

SvSelectClassDialog::SvSelectClassDialog(QWidget *parent, bool showEditDelete, bool showSelect) :
  QDialog(parent)
{
  setupUi();
    
  bnEditItem->setVisible(showEditDelete);
  bnDeleteItem->setVisible(showEditDelete);
  bnSelect->setVisible(showSelect);
  
  connect(bnSelect, SIGNAL(pressed()), this, SLOT(accept()));
  connect(bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(bnNewItem, SIGNAL(pressed()), this, SLOT(slotNewItem()));
  connect(bnEditItem, SIGNAL(pressed()), this, SLOT(slotEditItem()));
  connect(bnDeleteItem, SIGNAL(pressed()), this, SLOT(slotDeleteItem()));  
  
  loadItems();
  
  this->setModal(true);
  this->show();
  
}

SvSelectClassDialog::~SvSelectClassDialog()
{
  this->close();
  this->deleteLater();
}

void SvSelectClassDialog::accept()
{
  if(cbItemsList->count()) 
  {
    t_id = cbItemsList->currentData().toInt();
    t_class_name = cbItemsList->currentText();
  }  
  
  QDialog::accept();
  
}

void SvSelectClassDialog::slotNewItem()
{
  DEVCLASS_UI = new SvDeviceClass(this);
  if(DEVCLASS_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEVCLASS_UI->t_class_name));
  }
  DEVCLASS_UI->~SvDeviceClass();
}

void SvSelectClassDialog::slotEditItem()
{
  if(cbItemsList->currentData().isNull()) return;
  
  DEVCLASS_UI = new SvDeviceClass(this, cbItemsList->currentData().toInt(), smEdit);
  if(DEVCLASS_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEVCLASS_UI->t_class_name));
  }
  DEVCLASS_UI->~SvDeviceClass();
}

void SvSelectClassDialog::slotDeleteItem()
{

}

void SvSelectClassDialog::loadItems()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_CLASSES_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    QPixmap pix = QPixmap(16, 16);
    pix.loadFromData(q->value("icon").toByteArray());
    cbItemsList->addItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)),
                         q->value("class_name").toString(),
                         q->value("class_id").toInt());
  }
  q->finish();
  
  if(cbItemsList->count()) cbItemsList->setCurrentIndex(0);

}

void SvSelectClassDialog::setupUi()
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
