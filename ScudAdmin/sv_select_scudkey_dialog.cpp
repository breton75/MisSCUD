#include "sv_select_scudkey_dialog.h"

SvSelectScudkeyDialog *SELECTSCUDKEY_UI;
extern SvSCUD *SCUD_UI;
extern SvPGDB *PGDB;

SvSelectScudkeyDialog::SvSelectScudkeyDialog(QWidget *parent, bool showCreate, bool showEditDelete) :
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

SvSelectScudkeyDialog::~SvSelectScudkeyDialog()
{
  this->close();
  this->deleteLater();
}

void SvSelectScudkeyDialog::accept()
{
  if(cbItemsList->count()) 
  {
    t_id = cbItemsList->currentData().toInt();
    t_key_value = cbItemsList->currentText();
  }  
  
  QDialog::accept();
  
}

void SvSelectScudkeyDialog::slotNewItem()
{
  SCUD_UI = new SvSCUD(this);
  if(SCUD_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(QString::number(SCUD_UI->t_key_value)));
  }
  SCUD_UI->~SvSCUD();
}

void SvSelectScudkeyDialog::slotEditItem()
{
  if(cbItemsList->currentData().isNull()) return;
  
  SCUD_UI = new SvSCUD(this, cbItemsList->currentData().toInt());
  if(SCUD_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(QString::number(SCUD_UI->t_key_value)));
  }
  SCUD_UI->~SvSCUD();
}

void SvSelectScudkeyDialog::slotDeleteItem()
{

}

void SvSelectScudkeyDialog::loadItems()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_SCUDKEYS_LIST).arg("where employee_id = -1"), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    QPixmap pix = QPixmap(16, 16);
    pix.load(":/employee/icons/silk/key.png");
    cbItemsList->addItem(QIcon(pix),
                         q->value("key_value").toString(),
                         q->value("scud_id").toInt());
  }

  q->finish();
  
  if(cbItemsList->count()) cbItemsList->setCurrentIndex(0);
  
  bnSelect->setEnabled(!cbItemsList->currentData().isNull());
  bnEditItem->setEnabled(!cbItemsList->currentData().isNull());
  bnDeleteItem->setEnabled(!cbItemsList->currentData().isNull());
}

void SvSelectScudkeyDialog::setupUi()
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

  this->setWindowTitle("Выберите ключ СКУД");
  bnNewItem->setText("Создать");
  bnEditItem->setText("Изменить");
  bnDeleteItem->setText("Удалить");
  bnSelect->setText("Выбрать");
  bnCancel->setText("Отмена");
}
