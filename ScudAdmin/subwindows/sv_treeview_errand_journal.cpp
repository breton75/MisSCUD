#include "sv_treeview_errand_journal.h"

using namespace log_ns;

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;
extern SvErrand* ERRAND_UI;

SvTreeViewErrandJournal::SvTreeViewErrandJournal(QWidget *parent,
                               int departmentId,
                               int employeeId,
                               QDate periodBegin,
                               QDate periodEnd,
                               QMdiArea *mdiWindow)
//  QWidget(parent)
{
  this->setParent(parent);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  this->departmentId = departmentId;
  this->employeeId = employeeId;
  this->periodBegin = periodBegin;
  this->periodEnd = periodEnd;
  
//  /** *** создаем модель и таблицу данных *** **/
//  // создаем модель данных  
//  QStringList headers = QStringList(); //HEADER_REPORT_MAIN).split(';');
 
//  this->model = new SvModelMain(headers, this);
  this->setupModelData();
//  this->setModel(model);
//  this->expandToDepth(0);
  
//  for(int i = 0; i < headers.count(); i++)
//  {
//    this->setColumnHidden(i, headers.at(i).startsWith('-'));
//    this->resizeColumnToContents(i);
//  }

  QIcon icon;
  QFont fnt;
  fnt.setBold(true);
  
  /** *********** actions ************** **/
  icon.addFile(QStringLiteral(":/absence_journal/icons/cologne-icons/ico/expand.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionExpandAll = new QAction(this);
  actionExpandAll->setObjectName(QStringLiteral("actionExpandAll"));
  actionExpandAll->setIcon(icon);
  actionExpandAll->setText("Развернуть");
  actionExpandAll->setEnabled(true);
  actionExpandAll->setToolTip("Развернуть");
  actionExpandAll->setFont(fnt);
  actionExpandAll->setCheckable(true);
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/cologne-icons/ico/plus.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionNewErrandJornal = new QAction(this);
  actionNewErrandJornal->setObjectName(QStringLiteral("actionNewErrandJornal"));
  actionNewErrandJornal->setIcon(icon);
  actionNewErrandJornal->setText("Добавить");
  actionNewErrandJornal->setToolTip("Добавить новую запись в журнал");
  actionNewErrandJornal->setEnabled(true);
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/cologne-icons/ico/edit.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditErrandJornal = new QAction(this);
  actionEditErrandJornal->setObjectName(QStringLiteral("actionEditErrandJornal"));
  actionEditErrandJornal->setIcon(icon);
  actionEditErrandJornal->setText("Изменить");
  actionEditErrandJornal->setToolTip("Изменить запись в журнале");
  actionEditErrandJornal->setEnabled(false);  
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/silk/cross.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionDeleteErrandJornal = new QAction(this);
  actionDeleteErrandJornal->setObjectName(QStringLiteral("actionDeleteErrandJornal"));
  actionDeleteErrandJornal->setIcon(icon);
  actionDeleteErrandJornal->setText("Удалить");
  actionDeleteErrandJornal->setToolTip("Удалить запись");
  actionDeleteErrandJornal->setEnabled(false);  
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/cologne-icons/ico/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReload = new QAction(this);
  actionReload->setObjectName(QStringLiteral("actionReload"));
  actionReload->setIcon(icon);
  actionReload->setText("Обновить"); 
  actionReload->setEnabled(true);
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/excel16.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionCreateXLS = new QAction(this);
  actionCreateXLS->setObjectName(QStringLiteral("actionCreateXLS"));
  actionCreateXLS->setIcon(icon);
  actionCreateXLS->setText("Выгрузить в Excel");
  actionCreateXLS->setToolTip("Выгрузить в Excel");
  actionCreateXLS->setEnabled(true);  
  
  QMetaObject::connectSlotsByName(this);
  
  show();
  
}

SvTreeViewErrandJournal::~SvTreeViewErrandJournal()
{
  this->deleteLater();
}

void SvTreeViewErrandJournal::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu* menu = new QMenu(this);
  
  menu->insertAction(0,actionNewErrandJornal);
  menu->insertAction(0,actionEditErrandJornal);
  menu->insertAction(0,actionDeleteErrandJornal);
  menu->addSeparator();
  menu->insertAction(0, actionCreateXLS);
  menu->addSeparator();
  menu->insertAction(0, actionReload);
  menu->setTitle("Журнал");
  
  menu->exec(event->globalPos());
}

void SvTreeViewErrandJournal::mouseDoubleClickEvent(QMouseEvent * event)
{
  if(model->itemFromIndex(this->currentIndex())->item_type == itErrandJournal)
    on_actionEditErrandJornal_triggered();
  
  QTreeView::mouseDoubleClickEvent(event);
}

void SvTreeViewErrandJournal::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  TreeItem* item = model->itemFromIndex(current);
  
  switch (item->item_type)
  {
    case itErrandJournal:
      actionEditErrandJornal->setEnabled(true);
      actionDeleteErrandJornal->setEnabled(true);
      break;
      
    default:
      actionEditErrandJornal->setEnabled(false);
      actionDeleteErrandJornal->setEnabled(false);
      break;
  }

  QTreeView::currentChanged(current, previous);
}

void SvTreeViewErrandJournal::on_actionExpandAll_triggered()
{
  if(expanded)
    this->expandToDepth(0);
  else
    this->expandAll();

  expanded = !expanded;  
  actionExpandAll->setChecked(expanded);
}

/** *************** ЗАГРУЖАЕМ ДАННЫЕ ****************** **/
void SvTreeViewErrandJournal::setupModelData()
{
  showBusy("Формирование журнала. Подождите.");  
  connect(this, SIGNAL(reportCreated()), dlgBusy, SLOT(accept()));
  qApp->processEvents();
  
  
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_REPORT_ERRAND_JOURNAL)
                                         .arg(this->employeeId)
                                         .arg(this->departmentId)
                                         .arg(this->periodBegin.toString("dd/MM/yyyy"))
                                         .arg(this->periodEnd.toString("dd/MM/yyyy")), q).type())
  {
    q->finish(); 
    delete q;
    emit reportCreated();
    return;
  }
  
  // читаем заголовки колонок
  q->first();

  QStringList headers = QStringList();
  for(int i = 0; i < q->record().count(); i++)
  {
    if(q->value(i).isNull()) headers << "-<";
    else headers << q->value(i).toString();
  }

  // создаем модель данных  
  this->model = new SvModelMain(headers, this);
  
  // формируем отчет
  // rootItem создается при создании объекта модели
  TreeItem *parent = model->root();
  ItemInfo inf;
  
  int child_count = 0;
  int column_count = parent->columnCount();
  
  int last_level = 0;
  TreeItem* last_parent = parent;
  while(q->next())
  {
    if(q->value("lvl").toInt() > last_level)
    {
      last_parent = parent;
      parent = parent->child(child_count);
    }
    else if(q->value("lvl").toInt() < last_level)
    {
      for(int i = 0; i < (last_level - q->value("lvl").toInt()); i++)
        parent = parent->parent();
      last_parent = parent;
    }
     
    
    child_count = parent->childCount();
 

    parent->insertChildren(child_count, 1, column_count);
    
//    parent->child(child_count)->is_main_row = q->value("is_main_row").toBool();
    parent->child(child_count)->id = q->value("id").toInt();
    
    if(q->value("duty").toString() == "department") parent->child(child_count)->item_type = itDepartment;
    else if(q->value("duty").toString() == "errand_journal") parent->child(child_count)->item_type = itErrandJournal;
    
    parent->child(child_count)->bgnd_color = q->value("bgnd_color").toInt();
    parent->child(child_count)->font_color = q->value("font_color").toInt();
    parent->child(child_count)->font_bold = q->value("font_bold").toInt();
    parent->child(child_count)->font_italic = q->value("font_italic").toInt();
    
    for(int i = 0; i < q->record().count(); i ++)
    {
      inf.fieldName = q->record().fieldName(i);
      parent->child(child_count)->setData(i, q->value(inf.fieldName).toString()); // q->value("employee_short_name").toString());
      parent->child(child_count)->setInfo(i, inf);
    }
    
    last_level = q->value("lvl").toInt();
//    if(q->value("is_main_row").toBool()/* && (q->value("id").toInt() != parent->id)*/) 
//      parent = model->root()->child(child_count);
//    else if(q->value("id").toInt() != parent->id)
//      parent = model->root();    
  }

  q->finish();
  free(q); 
  
  this->setModel(model);
  this->expandToDepth(0);
  
  for(int i = 0; i < headers.count(); i++)
  {
    this->setColumnHidden(i, headers.at(i).startsWith('-'));
    this->resizeColumnToContents(i);
  }
//  qDebug() << headers.count();
  emit reportCreated();

}

void SvTreeViewErrandJournal::showBusy(QString text)
{
  dlgBusy = new QDialog(this, Qt::Window | Qt::FramelessWindowHint);
  dlgBusy->resize(300, 100);
  dlgBusy->setStyleSheet(QStringLiteral("background-color: rgb(90, 170, 255);"));
  connect(dlgBusy, SIGNAL(accepted()), dlgBusy, SLOT(deleteLater()));
  
  QVBoxLayout* vlay = new QVBoxLayout(dlgBusy);
  
  QGroupBox* gbox = new QGroupBox(dlgBusy);
  gbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gbox->setTitle("");
  
  QVBoxLayout* vlay2 = new QVBoxLayout(gbox);
  
  QLabel* lbl = new QLabel(gbox);
  lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  lbl->setText(text);
  
  vlay->addWidget(gbox);
  vlay2->addWidget(lbl);
  
  dlgBusy->setModal(true);
  dlgBusy->show();
}

void SvTreeViewErrandJournal::on_actionReload_triggered()
{
  model->clear();
  this->setupModelData();
}

/** ********* РЕДАКТИРОВАНИЕ ЗАПИСЕЙ ЖУРНАЛА *************** **/
void SvTreeViewErrandJournal::on_actionNewErrandJornal_triggered()
{
  ERRAND_UI = new SvErrand(this);
  if(ERRAND_UI->exec() == QDialog::Accepted)
  {
    model->clear();
    setupModelData();
  }
  ERRAND_UI->~SvErrand();

}

void SvTreeViewErrandJournal::on_actionEditErrandJornal_triggered()
{
  ERRAND_UI = new SvErrand(this, model->itemFromIndex(this->currentIndex())->id);
  if(ERRAND_UI->exec() == QDialog::Accepted)
  {
    
  }
  ERRAND_UI->~SvErrand();
}

void SvTreeViewErrandJournal::on_actionDeleteErrandJornal_triggered()
{
  PGDB->execSQL(QString("delete from errand_journal where id = %1")
                .arg(model->itemFromIndex(currentIndex())->id));
  
  on_actionReload_triggered();
}

/** выгрузка в excel **/
void SvTreeViewErrandJournal::on_actionCreateXLS_triggered()
{
  if(!model->root()->childCount()) return;

  showBusy("Формирование отчета. Подождите.");  
  connect(this, SIGNAL(reportCreated()), dlgBusy, SLOT(accept()));
  qApp->processEvents();
  
  // формируем список элементов дерева
  QList<TreeItem*> items = QList<TreeItem*>();
  QList<TreeItem*> temp = QList<TreeItem*>();
  TreeItem* parent;
  
  int counter = 0;
  int main_row_counter = 0;
  QString numeric;
  QMap<TreeItem*, QString> imap;
  
  items << model->root();
  temp  << model->root();
  while(temp.count())
  {
    parent = temp.first();
    counter = 0;
    for(int i = 0; i < parent->childCount(); i++)
    {
      temp  << parent->child(i);
      items << parent->child(i);
    }
    temp.pop_front();
  }

  // теперь необходимо расставить элементы дерева в нужном порядке
  while(items.count())
  {
    parent = items.first();
    int parent_index = temp.indexOf(parent) + 1;
    counter = 0;
    for(int i = 0; i < items.count(); i++)
    {
      
      if(items.at(i)->parent() == parent)
      {
        temp.insert(parent_index, items.at(i));
        parent_index++;
        
        if(items.at(i)->parent() == model->root())
        {
          main_row_counter++;
          
          numeric = QString::number(main_row_counter);
        }
        else
        {
          counter++;
          numeric = imap.value(parent) + "." + QString::number(counter);
        }
        imap.insert(items.at(i), numeric);
        
      }
    }
    items.pop_front();
  }

//    ////////////////////////////////////////////////////////

  EXCEL *excl = new EXCEL;

  /////////////////////////////////////////////////////////

  QList <QVariant> shap = QList <QVariant>();

  int col = 1, zdv = 1;

  /* рисуем шапку */
  shap << "№";
  for(int i = 0; i < model->root()->columnCount(); i++)
  {
    if(!this->isColumnHidden(i))
    {
      shap << model->root()->data(i);
      col++;
    }
  }
  excl->EXCEL_Range(shap, 1, 1, 1, col, true, false, 0, 0xFFFFFF, EXCEL::Border_xlMedium);

  /* выкладываем данные */
  for(int row = 0; row < temp.count(); row++)
  {
    shap.clear();
    shap << row; // imap.value(items.at(row));
//    qDebug() << "row="<<row;
    for(int column = 0; column < temp.at(row)->columnCount(); column++)
    {
      if(!this->isColumnHidden(column))
      {
        if(temp.at(row)->data(column).isNull()) shap <<  QVariant("");
        else shap << temp.at(row)->data(column);
      }
      
    }
    excl->EXCEL_Range(shap,
                      row + 1 + zdv, 1, row + 1 + zdv, col,
                      temp.at(row)->font_bold, temp.at(row)->font_italic,
                      temp.at(row)->font_color, temp.at(row)->bgnd_color,
                      EXCEL::Border_xlThin);
    
  }

  excl->AutoFit(1, 100);
  excl->Visible();
  
  emit this->reportCreated();

}
