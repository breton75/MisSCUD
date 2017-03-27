#include "sv_treeview_main.h"
#include <QTextCodec>
#ifdef _WIN32
  #include <windows.h>
#endif

using namespace log_ns;
using namespace libxl;

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;
//extern SvTask* TASK_UI;
extern SvEmployee* EMPLOYEE_UI;
//extern SvProjectsGeneralTable* GENERAL_WINDOW_UI;
//extern SvReportEmployeeByProject* REPORT_EMPLOYEE_WINDOW_UI;

SvTreeViewMain::SvTreeViewMain(QWidget *parent,
                               int departmentId,
                               int employeeId,
                               QDate periodBegin,
                               QDate periodEnd,
                               int reportType,
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
  this->report_type = reportType;
  
//  /** *** создаем модель и таблицу данных *** **/
//  // создаем модель данных  
  QStringList headers = QStringList(); //HEADER_REPORT_MAIN).split(';');
 
//  this->model = new SvModelMain(headers, this);
  this->setupModelData(headers);
  this->setModel(model);
  if(employeeId != -1) this->expandToDepth(1);
  else this->expandToDepth(0);
  
  for(int i = 0; i < headers.count(); i++)
  {
    this->setColumnHidden(i, headers.at(i).startsWith('-'));
    this->resizeColumnToContents(i);
  }

  QIcon icon;
  QFont fnt;
  fnt.setBold(true);
  
  /** *********** actions ************** **/
  icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/expand.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionExpandAll = new QAction(this);
  actionExpandAll->setObjectName(QStringLiteral("actionExpandAll"));
  actionExpandAll->setIcon(icon);
  actionExpandAll->setText("Развернуть");
  actionExpandAll->setEnabled(true);
  actionExpandAll->setToolTip("Развернуть");
  actionExpandAll->setFont(fnt);
  actionExpandAll->setCheckable(true);
  
//  icon.addFile(QStringLiteral(":/tree32/icons/tree32/document.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditEmployee = new QAction(this);
  actionEditEmployee->setObjectName(QStringLiteral("actionEditEmployee"));
//  actionEditEmployee->setIcon(icon);
  actionEditEmployee->setText("Редактировать");
  actionEditEmployee->setToolTip("Редактировать данные сотрудника");
  actionEditEmployee->setEnabled(false);
  
  icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReload = new QAction(this);
  actionReload->setObjectName(QStringLiteral("actionReload"));
  actionReload->setIcon(icon);
  actionReload->setText("Обновить"); 
  
  icon.addFile(QStringLiteral(":/reports/icons/excel16.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionCreateXLS = new QAction(this);
  actionCreateXLS->setObjectName(QStringLiteral("actionCreateXLS"));
  actionCreateXLS->setIcon(icon);
  actionCreateXLS->setText("Выгрузить в Excel");
  actionCreateXLS->setToolTip("Выгрузить в Excel");
  actionCreateXLS->setEnabled(true);  
  
  QMetaObject::connectSlotsByName(this);
  
  show();
  
}

SvTreeViewMain::~SvTreeViewMain()
{
  this->deleteLater();
}

void SvTreeViewMain::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu* menu = new QMenu(this);
  
  menu->insertAction(0, actionCreateXLS);
  menu->addSeparator();
  menu->insertAction(0, actionReload);
  menu->setTitle("Отчет");
  
//  switch (this->model->itemFromIndex(this->currentIndex())->item_type)
//  {
//    case itEmployeeWorkTime:
//    {

//      break;
//    }
//  }
  
  menu->exec(event->globalPos());
}

void SvTreeViewMain::mouseDoubleClickEvent(QMouseEvent * event)
{

  QTreeView::mouseDoubleClickEvent(event);
}

void SvTreeViewMain::mousePressEvent(QMouseEvent *event)
{

  QTreeView::mousePressEvent(event);

}

void SvTreeViewMain::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  TreeItem* item = model->itemFromIndex(current);
  
  switch (item->item_type)
  {
    case itEmployeeWorkTime:
//      actionEditEmployee->setEnabled(true);
      actionCreateXLS->setEnabled(true);
      actionReload->setEnabled(true);
      break;
      
    default:
//      actionEditEmployee->setEnabled(false);
      actionCreateXLS->setEnabled(true);
      actionReload->setEnabled(true);
      break;
  }

  QTreeView::currentChanged(current, previous);
}

void SvTreeViewMain::on_actionExpandAll_triggered()
{
  if(expanded)
    this->expandToDepth(0);
  else
    this->expandAll();

  expanded = !expanded;  
  actionExpandAll->setChecked(expanded);
}

/** *************** ЗАГРУЖАЕМ ДАННЫЕ ****************** **/
void SvTreeViewMain::setupModelData(QStringList &headers)
{
  showBusy("Формирование отчета. Подождите.");  
  connect(this, SIGNAL(reportCreated()), dlgBusy, SLOT(accept()));
  qApp->processEvents();
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_REPORT_GENERAL)
                                         .arg(this->employeeId)
                                         .arg(this->departmentId)
                                         .arg(this->periodBegin.toString("dd/MM/yyyy"))
                                         .arg(this->periodEnd.toString("dd/MM/yyyy"))
                                         .arg(this->report_type), q).type())
  {
    q->finish(); 
    emit reportCreated();
    return;
  }
  
  // читаем заголовки колонок
  q->first();
//  QStringList headers = QStringList();
  
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
    parent->child(child_count)->bgnd_color = q->value("bgnd_color").toInt();
    parent->child(child_count)->font_color = q->value("font_color").toInt();
    parent->child(child_count)->font_bold = q->value("font_bold").toInt();
    parent->child(child_count)->font_italic = q->value("font_italic").toInt();
    
    for(int i = 0; i < q->record().count(); i ++)
    {
      inf.fieldName = q->record().fieldName(i);
      QString s = q->value(inf.fieldName).toString();
      
      int pos1 = s.indexOf("{style:");
      int pos2 = s.indexOf('}');
      if((pos1 != -1) && (pos2 > pos1))
      {
        QString style = s.mid(pos1, pos2 - pos1 + 1);
        parent->child(child_count)->setStyle(i, style);
        s.remove(style);
      }
      
      parent->child(child_count)->setData(i, s);
      parent->child(child_count)->setInfo(i, inf);
    }
    
    last_level = q->value("lvl").toInt();
//    if(q->value("is_main_row").toBool()/* && (q->value("id").toInt() != parent->id)*/) 
//      parent = model->root()->child(child_count);
//    else if(q->value("id").toInt() != parent->id)
//      parent = model->root();    
  }
  
  emit reportCreated();
    
  q->finish();
  free(q);  

}

void SvTreeViewMain::showBusy(QString text)
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

void SvTreeViewMain::on_actionReload_triggered()
{
  model->clear();
  
  QStringList headers = QStringList(); //HEADER_REPORT_MAIN).split(';');
 
  this->setupModelData(headers);
  this->setModel(model);
  this->expandToDepth(0);
  
  for(int i = 0; i < headers.count(); i++)
  {
    this->setColumnHidden(i, headers.at(i).startsWith('-'));
    this->resizeColumnToContents(i);
  }

}

/** *********** СОТРУДНИКИ ****************** **/
void SvTreeViewMain::on_actionEditEmployee_triggered()
{
//  EMPLOYEE_UI = new SvEmployee(this, model->itemFromIndex(this->currentIndex())->id);
//  connect(EMPLOYEE_UI, SIGNAL(accepted()), this, SLOT(on_EMPLOYEE_UI_closed()));
//  connect(EMPLOYEE_UI, SIGNAL(rejected()), this, SLOT(on_EMPLOYEE_UI_closed()));
}

void SvTreeViewMain::on_actionCreateXLS_triggered()
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
