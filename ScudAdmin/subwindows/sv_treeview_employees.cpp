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
                               QDateTime periodBegin,
                               QDateTime periodEnd,
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
  
  /** *** создаем модель и таблицу данных *** **/
  // создаем модель данных  
  QStringList headers = QString(HEADER_REPORT_MAIN).split(';');
 
  this->model = new SvModelMain(headers, this);
  this->setupModelData();
  this->setModel(model);
  
  for(int i = 0; i < headers.count(); i++)
  {
    this->setColumnHidden(i, headers.at(i).startsWith('-'));
    this->resizeColumnToContents(i);
  }

  QIcon icon;
  QFont fnt;
  fnt.setBold(true);
  
  /** *********** actions ************** **/
  icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/old-versions.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionLoadDetails = new QAction(this);
  actionLoadDetails->setObjectName(QStringLiteral("actionLoadDetails"));
  actionLoadDetails->setIcon(icon);
  actionLoadDetails->setText("Подробно...");
  actionLoadDetails->setEnabled(false);
  actionLoadDetails->setToolTip("Загрузить подробности");
  actionLoadDetails->setFont(fnt);
  
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
  
  switch (this->model->itemFromIndex(this->currentIndex())->item_type)
  {
    case itEmployeeWorkTime:
    {
      menu->insertAction(0, actionLoadDetails);
      menu->addSeparator();
//      menu->insertAction(0, actionEditEmployee);
//      menu->addSeparator();
      menu->insertAction(0, actionCreateXLS);
      menu->addSeparator();
      menu->insertAction(0, actionReload);
      menu->setTitle("Сотрудники");
      break;
    }
  }
  
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
      actionLoadDetails->setEnabled(true);
//      actionEditEmployee->setEnabled(true);
      actionCreateXLS->setEnabled(true);
      actionReload->setEnabled(true);
      break;
      
    default:
      actionLoadDetails->setEnabled(false);
//      actionEditEmployee->setEnabled(false);
      actionCreateXLS->setEnabled(true);
      actionReload->setEnabled(true);
      break;
  }

  QTreeView::currentChanged(current, previous);
}

/** *************** ЗАГРУЖАЕМ ДАННЫЕ ****************** **/
void SvTreeViewMain::setupModelData()
{
  QList<TreeItem*> records;
  
  // rootItem создается при создании объекта модели
  TreeItem *parent = model->root();
  ItemInfo inf;
  
  int child_count;
  int column_count = parent->columnCount();
  
//  // определяем состояние проекта
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  
  // создаем записи для сотрудников
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_REPORT_MAIN)
                                         .arg(this->employeeId)
                                         .arg(this->departmentId)
                                         .arg(this->periodBegin.toString("dd/MM/yyyy hh:mm:ss"))
                                         .arg(this->periodEnd.toString("dd/MM/yyyy hh:mm:ss")), q).type())
  {
    q->finish(); 
    return;
  }

  while(q->next())
  {
    child_count = parent->childCount();
    
    parent->insertChildren(child_count, 1, column_count);
    
    parent->child(child_count)->item_type = itEmployeeWorkTime;
    parent->child(child_count)->id = q->value("employee_id").toInt();
    parent->child(child_count)->is_main_row = false;
    
    inf.fieldName = "employee_id";
    inf.type = itEmployeeId;
    parent->child(child_count)->setData(0, q->value("employee_id").toInt());
    parent->child(child_count)->setInfo(0, inf);
    
    inf.fieldName = "employee_short_name";
    inf.type = itEmployeeName;
    parent->child(child_count)->setData(1, q->value("employee_short_name").toString());
    parent->child(child_count)->setInfo(1, inf);
    
    inf.fieldName = "total_work_time";
    inf.type = itEmployeeWorkTime;
    parent->child(child_count)->setData(2, q->value("total_work_time").toTime().toString("hh час. mm мин."));
    parent->child(child_count)->setInfo(2, inf); 
    
    inf.fieldName = "employee_position";
    inf.type = itEmployeePosition;
    parent->child(child_count)->setData(3, q->value("department_name").toString());
    parent->child(child_count)->setInfo(3, inf);
    
    inf.fieldName = "department_name";
    inf.type = itEmployeeDepartment;
    parent->child(child_count)->setData(4, q->value("employee_position").toString());
    parent->child(child_count)->setInfo(4, inf);
    
    records << parent->child(child_count);
  }
  
  if(this->employeeId != -1)
  {
    loadDetails(records.first());
    this->expand(model->index(0, 0));
  }

  // добавляем для каждого сотрудника список задач текущего проекта
//  while(records.count())
//  {
//    parent = records.first();
    
//    // делаем выборку задач текущего проекта, в которых задействыован сотрудник
//    q->finish();
////    err = PGDB->execSQL(QString(SQL_REPORT_EMP_BY_PROJ)
////                        .arg(parent->id)
////                        .arg(projectId), q);
    
//    while(q->next())
//    {
//      child_count = parent->childCount();
      
//      parent->insertChildren(child_count, 1, column_count);
      
//      parent->child(child_count)->id = q->value("task_id").toInt();
//      parent->child(child_count)->is_main_row = false;
////      parent->child(child_count)->item_type = itTask;
//      parent->child(child_count)->item_state = q->value("task_state").toInt();
      
//      for (int column = 0; column < column_count; column++)
//      {
//        QString field_name = q->record().fieldName(column);

////        if(field_name == "task_id") inf.type = itTaskId;
////        else if(field_name == "task_name") inf.type = itTaskName;
////        else if(field_name == "laboriousness") inf.type = itTaskLaboriousness;
////        else if(field_name == "allot_laboriousness") inf.type = itEmployeeLoadAllot;
////        else if(field_name == "fact_laboriousness") inf.type = itEmployeeLoadFact;
////        else if(field_name == "task_added") inf.type = itTaskAdded;
////        else if(field_name == "task_started") inf.type = itTaskStarted;
////        else if(field_name == "author") inf.type = itTaskAuthor;
////        else if(field_name == "task_state") inf.type = itTaskState;
////        else inf.type = itUndefined;
        
//        inf.fieldName = field_name;

//        parent->child(child_count)->setData(column + 1, q->value(field_name));
//        parent->child(child_count)->setInfo(column + 1, inf);
        
//      }
//    }
    
//    records.pop_front();
//  }
    
  q->finish();
  free(q);  

}


void SvTreeViewMain::on_actionLoadDetails_triggered()
{

  loadDetails(model->itemFromIndex(this->currentIndex()));

}

void SvTreeViewMain::loadDetails(TreeItem* parent)
{
//  TreeItem *parent = model->itemFromIndex(this->currentIndex());
  ItemInfo inf;
  
  int child_count;
  int column_count = parent->columnCount();
  
  // определяем состояние проекта
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  
  // создаем записи для сотрудников
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_REPORT_MAIN_DETAILS)
                                         .arg(parent->id)
                                         .arg(this->periodBegin.toString("dd/MM/yyyy hh:mm:ss"))
                                         .arg(this->periodEnd.toString("dd/MM/yyyy hh:mm:ss")), q).type())
  {
    q->finish(); 
    return;
  }

  while(q->next())
  {
    child_count = parent->childCount();
    
    parent->insertChildren(child_count, 1, column_count);
    
    parent->child(child_count)->item_type = itScudReport;
    parent->child(child_count)->id = parent->id;
    parent->child(child_count)->is_main_row = false;
    parent->child(child_count)->item_state = q->value("sensor_number").toInt();
    
    inf.fieldName = "";
    inf.type = itUndefined;
    parent->child(child_count)->setData(0, "");
    parent->child(child_count)->setInfo(0, inf);
    
    inf.fieldName = "device_name";
    inf.type = itScudDeviceName;
    parent->child(child_count)->setData(1, q->value("device_name").toString());
    parent->child(child_count)->setInfo(1, inf);
    
    inf.fieldName = "date_time";
    inf.type = itEmployeePosition;
    parent->child(child_count)->setData(2, q->value("date_time").toDateTime().toString("dd/MM/yyyy hh:mm"));
    parent->child(child_count)->setInfo(2, inf);
    
    inf.fieldName = "";
    inf.type = itUndefined;
    parent->child(child_count)->setData(3, "");
    parent->child(child_count)->setInfo(3, inf);
    
    inf.fieldName = "";
    inf.type = itUndefined;
    parent->child(child_count)->setData(4, "");
    parent->child(child_count)->setInfo(4, inf); 
    
//     records << parent->child(child_count);
  }
     
  q->finish();


 if(parent->childCount())
 {
   parent->is_main_row = true;
   this->expand(this->currentIndex());
 }
}

void SvTreeViewMain::on_actionReload_triggered()
{
  
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
//  if(!model->root()->childCount()) return;
  
//  // читаем информацию о проекте
//  QSqlQuery* q = new QSqlQuery(PGDB->db);
//  QSqlError err = PGDB->execSQL(QString("select * from projects where id = %1").arg(projectId), q);
//  if(err.type() != QSqlError::NoError)
//  {
//    q->finish();
//    delete q;
//    return;
//  }
  
//  q->first();

////  int projectId;  
//  QString projectName = q->value("project_name").toString().toUtf8();
//  QString projectCode = q->value("project_code").toString();
//  int projectState = q->value("state").toInt();
//  QDate projectBegin = q->value("project_begin").toDate();
//  QDate projectEnd = q->value("project_end").toDate();
////  float projectLaboriouness;
////  float projectPlanSum;
////  float projectFactSum;
  
//  q->finish();
//  delete q;
  
//  Book* book = xlCreateBook();
  
//  Sheet* sheet = book->addSheet("Invoice");
//  if(sheet)
//  {
    
//   // sheet->
//    // формируем список элементов дерева
//    QList<TreeItem*> items = QList<TreeItem*>();
//    QList<TreeItem*> temp = QList<TreeItem*>();
//    TreeItem* parent;
    
//    int counter = 0;
//    int main_row_counter = 0;
//    QString numeric;
//    QMap<TreeItem*, QString> imap;
    
//    items << model->root();
//    temp  << model->root();
//    while(temp.count())
//    {
//      parent = temp.first();
//      counter = 0;
//      for(int i = 0; i < parent->childCount(); i++)
//      {
//        temp  << parent->child(i);
//        items << parent->child(i);
//      }
//      temp.pop_front();
//    }
    
//    // теперь необходимо расставить элементы дерева в нужном порядке
 
//    while(items.count())
//    {
//      parent = items.first();
//      int parent_index = temp.indexOf(parent) + 1;
//      counter = 0;
//      for(int i = 1; i < items.count(); i++)
//      {
        
//        if(items.at(i)->parent() == parent)
//        {
//          temp.insert(parent_index, items.at(i));
//          parent_index++;
          
//          if(items.at(i)->parent() == model->root())
//          {
//            main_row_counter++;
            
//            numeric = QString::number(main_row_counter);
//          }
//          else
//          {
//            counter++;
//            numeric = imap.value(parent) + "." + QString::number(counter);
//          }
//          imap.insert(items.at(i), numeric);
          
//        }
//      }
//      items.pop_front();
//    }

//    ////////////////////////////////////////////////////////

 
//EXCEL *excl=new EXCEL;


//    //////////////////////////////////////////////////////////
//QList <QVariant> shap=QList <QVariant>();


//int col=1, zdv=1;
//shap<<"№";
//for(int q=0;q<model->root()->columnCount();q++)
//{

//    switch (temp.at(0)->info(q).type)
//    {
//    //case itTaskId:
//    case itTaskName:
//    case itTaskLaboriousness:
//    case itTaskSumAlloted:
//    case itTaskSumFact:
//    case itEmployeeLoadAllot:
//    case itEmployeeLoadFact:
//        shap<<model->root()->data(q);
//        col++;
//    }

//}

//excl->EXCEL_Range((shap),1,1,1, col-1,true,EXCEL::Border_xlMedium);
//for(int row = 0; row < temp.count(); row++)
//{
//shap.clear();
//shap<<imap.value(temp.at(row));

//  for(int column = 0; column < temp.at(row)->columnCount(); column++)
//  {
//      switch (temp.at(row)->info(column).type)
//      {
//      //case itTaskId:
//      case itTaskName:
//      case itTaskLaboriousness:
//      case itTaskSumAlloted:
//      case itEmployeeLoadAllot:
//          case itTaskSumFact:
//      case itEmployeeLoadFact:
//      shap<<temp.at(row)->data(column);
//          break;
//      }


//  }
//  excl->EXCEL_Range((shap),row+1+zdv,1,row+1+zdv, col-1,temp.at(row)->is_main_row,EXCEL::Border_xlThin);


//}

//  excl->AutoFit(1,100);
//  excl->Visible();
//  }


}
