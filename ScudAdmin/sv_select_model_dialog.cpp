#include "sv_select_model_dialog.h"
#include "sv_dev_model.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

SvSelectModelDialog *SELECTMODELDIALOG_UI;
extern SvDevModel *DEVMODEL_UI;
extern SvPGDB *PGDB;

SvSelectModelDialog::SvSelectModelDialog(QWidget *parent, bool showEditDelete, bool showSelect) :
  QDialog(parent)
{
  this->setupUi();
  
  treeItems->setHeaderHidden(true);
  treeItems->setEditTriggers(0);
  treeItems->setModel(treeItems->model);
//  treeItems->model = new QStandardItemModel(this);
//  treeItems->itemRoot = model->invisibleRootItem();

  bnEditItem->setVisible(showEditDelete);
  bnDeleteItem->setVisible(showEditDelete);
  bnSelect->setVisible(showSelect);
  
  QMetaObject::connectSlotsByName(this);
  
  connect(bnSelect, SIGNAL(pressed()), this, SLOT(accept()));
  connect(bnCancel, SIGNAL(pressed()), this, SLOT(reject()));
  connect(treeItems, SIGNAL(mouseDoubleClick()), this, SLOT(accept()));
//  connect(bnNewItem, SIGNAL(pressed()), this, SLOT(slotNewModel()));
//  connect(bnEditItem, SIGNAL(pressed()), this, SLOT(slotEditModel()));
//  connect(bnDeleteItem, SIGNAL(pressed()), this, SLOT(slotDeleteModel()));

  loadItems();

  // перенесено в loadItems()
//  treeItems->expandAll();
//  connect(treeItems, SIGNAL(currentChanged(QModelIndex)), this, SLOT(currentChanged(QModelIndex)));
  
  this->setModal(true);
  this->show();

}

void SvSelectModelDialog::currentChanged(const QModelIndex &current)
{
  if(!current.isValid()) return;
  bnSelect->setEnabled(treeItems->model->itemFromIndex(current)->type() == itDeviceModel);
  
}

void SvSelectModelDialog::setupUi()
{
  this->setObjectName(QStringLiteral("SelectModelDialog"));
  this->resize(600, 400);
  verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

  lblItems = new QLabel(this);
  lblItems->setObjectName(QStringLiteral("lblItems"));
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(lblItems->sizePolicy().hasHeightForWidth());
  lblItems->setSizePolicy(sizePolicy);
  lblItems->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  
  treeItems = new SvTreeModelsList(this);
  treeItems->setObjectName(QStringLiteral("treeItems"));
  treeItems->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  hlayButtons = new QHBoxLayout();
  hlayButtons->setObjectName(QStringLiteral("hlayButtons"));
  
  bnNewItem = new QPushButton(this);
  bnNewItem->setObjectName(QStringLiteral("bnNewItem"));
  
  bnEditItem = new QPushButton(this);
  bnEditItem->setObjectName(QStringLiteral("bnEditItem"));
  
  bnDeleteItem = new QPushButton(this);
  bnDeleteItem->setObjectName(QStringLiteral("bnDeleteItem"));
  
  spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  
  bnSelect = new QPushButton(this);
  bnSelect->setObjectName(QStringLiteral("bnSelect"));
  
  bnCancel = new QPushButton(this);
  bnCancel->setObjectName(QStringLiteral("bnCancel"));
  
  hlayButtons->addWidget(bnNewItem);
  hlayButtons->addItem(spacer1);
  hlayButtons->addWidget(bnSelect);
  hlayButtons->addWidget(bnCancel);

  verticalLayout->addWidget(lblItems);
  verticalLayout->addWidget(treeItems);
  verticalLayout->addLayout(hlayButtons);

  this->setWindowTitle(QApplication::translate("Dialog", "Выберите модель", 0));
  lblItems->setText(QApplication::translate("Dialog", "Выберите модель", 0));
  bnSelect->setText(QApplication::translate("Dialog", "Выбрать", 0));
  bnCancel->setText(QApplication::translate("Dialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
  bnNewItem->setText(QApplication::translate("Dialog", "Создать", 0));
}

SvSelectModelDialog::~SvSelectModelDialog()
{
  this->close();
  this->deleteLater();
}

void SvSelectModelDialog::accept()
{
  if(treeItems->model->itemFromIndex(treeItems->currentIndex())->type() != itDeviceModel)
    return;
  
  t_id = treeItems->model->data(treeItems->currentIndex(), roleId).toInt();
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_MODEL).arg(t_id), q).type())
  {
    q->finish();
    delete q;
    QDialog::reject();
    return;
  }
  q->first();
  
  t_model_name  = q->value("model_name").toString();
  t_class_id    = q->value("class_id").toInt();
  t_brand_id    = q->value("brand_id").toInt();
  t_class_name  = q->value("class_name").toString();
  t_brand_name  = q->value("brand_name").toString();
  t_driver_path = q->value("model_lib_path").toString();
  t_description = q->value("description").toString();

  q->finish();
  
  QDialog::accept();
  
}

void SvSelectModelDialog::on_bnNewItem_clicked()
{
  DEVMODEL_UI = new SvDevModel(this);
  if(DEVMODEL_UI->exec() == QDialog::Accepted)
  {
    loadItems(true, DEVMODEL_UI->t_id);
  }
  DEVMODEL_UI->~SvDevModel();

}

void SvSelectModelDialog::on_bnEditItem_clicked()
{
//  t_modelName = model->data(model->index(Dialog->tblModelsList->selectionModel()->currentIndex().row(), 0)).toString();
//  t_className = model->data(model->index(Dialog->tblModelsList->selectionModel()->currentIndex().row(), 1)).toString();
//  t_brandName = model->data(model->index(Dialog->tblModelsList->selectionModel()->currentIndex().row(), 2)).toString();
//  t_libPath = model->data(model->index(Dialog->tblModelsList->selectionModel()->currentIndex().row(), 3)).toString();
  
//  DEVMODEL_UI = new SvDevModel(this, t_modelName, t_brandName, t_className, t_libPath, "", 0, SvDevModel::smEdit);
//  connect(DEVMODEL_UI, SIGNAL(accepted()), this, SLOT(on_DEVMODEL_UI_closed()));
//  connect(DEVMODEL_UI, SIGNAL(rejected()), this, SLOT(on_DEVMODEL_UI_closed()));
}

void SvSelectModelDialog::on_bnDeleteItem_clicked()
{

}

void SvSelectModelDialog::loadItems(bool reload, int setToId)
{
  disconnect(treeItems, SIGNAL(currentChanged(QModelIndex)), this, SLOT(currentChanged(QModelIndex)));
  if(reload)
  {
    while(treeItems->itemRoot->rowCount())
    {
      treeItems->itemRoot->child(0)->removeRows(0, treeItems->itemRoot->child(0)->rowCount());
      treeItems->itemRoot->removeRow(0);
    }
  }

  // 1. загружаем список классов устройств
  // разбито на два этапа, чтобы небыло запросов, при обработке текущего запроса
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_CLASSES_LIST).arg(""), q).type())
  {
    q->finish();
    delete q;
    return;
  }
  
  QList<QStandardItem*> classes = QList<QStandardItem*>();
  while(q->next())
  {
    QPixmap pix = QPixmap();
    pix.loadFromData(q->value("icon").toByteArray());

    classes << new SvDeviceClassItem(QIcon(pix), q->value("class_name").toString());
    classes.last()->setData(q->value("class_id").toInt(), roleId);
    QFont fnt;
    fnt.setBold(true);
    classes.last()->setFont(fnt);
    treeItems->itemRoot->appendRow(classes.last());

  }
  q->finish();

  // 2. теперь загружаем список моделей по классам устройств
  while(classes.count())
  {
    QStandardItem* parent = classes.takeAt(0);

    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_MODELS_LIST)
                                           .arg(QString("where class_id = %1")
                                                .arg(parent->data(roleId).toInt()))
                                           , q).type())
    {
      q->finish();
      return;
    }
    
    while(q->next())
    {
      QPixmap pix = QPixmap();
      pix.load(":/tree/icons/cologne-icons/ico/full-time.ico");
    
      SvDeviceModelItem* item = new SvDeviceModelItem(QIcon(pix),
                                              QString("%1 %2")
                                                .arg(q->value("brand_name").toString())
                                                .arg(q->value("model_name").toString()));
      item->setData(q->value("model_id"), roleId);
      parent->appendRow(item);

    }
    q->finish();
  }

  delete q;
  
  treeItems->expandToDepth(0);
  connect(treeItems, SIGNAL(currentChanged(QModelIndex)), this, SLOT(currentChanged(QModelIndex)));
  
  if(setToId != -1)
  {
    bool found = false;
    for(int i = 0; i < treeItems->itemRoot->rowCount(); i++)
    {
      QStandardItem* parent = treeItems->itemRoot->child(i);
      for(int j = 0; j < parent->rowCount(); j++)
      {
        if(parent->child(j)->data(roleId).toInt() == setToId)
        {
          treeItems->setCurrentIndex(treeItems->model->indexFromItem(parent->child(j)));
          found = true;
          break;
        }
      }
      if(found) break;
    }
  }
  
  treeItems->setFocus();
  
}

SvTreeModelsList::SvTreeModelsList(QWidget *parent)
{
  this->setParent(parent);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  this->setHeaderHidden(true);

  this->setModel(model);
  this->setVisible(true);
  
}

SvTreeModelsList::~SvTreeModelsList()
{
  this->deleteLater();
}

void SvTreeModelsList::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  emit currentChanged(current);
}

void SvTreeModelsList::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit mouseDoubleClick();
  QTreeView::mouseDoubleClickEvent(event);
}
