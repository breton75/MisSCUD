#include "sv_select_brand_dialog.h"

SvSelectBrandDialog *SELECTBRAND_UI;

extern SvBrand *DEVBRAND_UI;
extern SvPGDB *PGDB;

SvSelectBrandDialog::SvSelectBrandDialog(QWidget *parent,
                                             bool showEditDelete,
                                             bool showSelect) :
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

SvSelectBrandDialog::~SvSelectBrandDialog()
{
  this->close();
  this->deleteLater();
}

void SvSelectBrandDialog::accept()
{
  if(cbItemsList->count()) 
  {
    t_id = cbItemsList->currentData().toInt();
    t_brand_name = cbItemsList->currentText();
  }  
  
  QDialog::accept();
}

void SvSelectBrandDialog::slotNewItem()
{
  DEVBRAND_UI = new SvBrand(this);
  if(DEVBRAND_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEVBRAND_UI->t_brand_name));
  }
  DEVBRAND_UI->~SvBrand();
}

void SvSelectBrandDialog::slotEditItem()
{
  if(cbItemsList->currentData().isNull()) return;
  
  DEVBRAND_UI = new SvBrand(this, cbItemsList->currentData().toInt());
  if(DEVBRAND_UI->exec() == QDialog::Accepted)
  {
    cbItemsList->clear();
    loadItems();
    cbItemsList->setCurrentIndex(cbItemsList->findText(DEVBRAND_UI->t_brand_name));
  }
  DEVBRAND_UI->~SvBrand();
}

void SvSelectBrandDialog::slotDeleteItem()
{
  
}

void SvSelectBrandDialog::loadItems()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL("select * from brands ORDER BY brand_name ASC", q).type())
  {
    q->finish();
    return;
  }
  
  cbItemsList->addItem(QIcon(), "<Пусто>", -1);
  
  while(q->next())
    cbItemsList->addItem(q->value("brand_name").toString(), q->value("brand_id").toInt());

  q->finish();
  
  if(cbItemsList->count()) cbItemsList->setCurrentIndex(0);

}

void SvSelectBrandDialog::setupUi()
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
